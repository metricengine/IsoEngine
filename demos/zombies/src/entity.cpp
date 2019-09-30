#include "entity.h"
#include "game.h"
#include "isoengine/support/resourcemanager.h"
#include <deque>
#include <set>

#include <iostream>

using iso::math::Vector2f;
using iso::math::Vector2i;

namespace
{

bool isValid(unsigned x, unsigned y, const Tile * map)
{
    if (x < 0 || y < 0 || x >= Game::LevelWidth || y >= Game::LevelHeight) {
        return false;
    }

    return map[y * Game::LevelWidth + x] == Tile::Grass;
}

Vector2f BFS(const Vector2f & orig, const Vector2f & target, const Tile * map)
{
    Vector2i tileOrig(orig.x / Entity::SpriteSize, orig.y / Entity::SpriteSize);
    Vector2i tileTarget(target.x / Entity::SpriteSize, target.y / Entity::SpriteSize);

    if ((tileOrig - tileTarget).length<double>() == 1.0) {
        return (target - orig).normalize();
    }

    // BFS
    auto cmp = [](const Vector2i & a, const Vector2i & b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    };
    std::set<Vector2i, decltype(cmp)> visited(cmp);
    // first -> original pos, second -> current pos
    std::deque<std::pair<Vector2i, Vector2i>> q;

    for (int i = tileOrig.y - 1; i <= tileOrig.y + 1; ++i) {
        for (int j = tileOrig.x - 1; j <= tileOrig.x + 1; ++j) {
            int c = int(i == tileOrig.y) + int(j == tileOrig.x);
            if (c != 1)
                continue;
            if (isValid(j, i, map)) {
                q.push_back(std::make_pair(Vector2i(j, i), Vector2i(j, i)));
                visited.emplace(j, i);
            }
        }
    }

    while (!q.empty()) {
        auto curr = q.front();
        auto & pos = curr.second;
        if (pos == tileTarget) {
            return {float(curr.first.x - tileOrig.x),
                    float(curr.first.y - tileOrig.y)};
        }
        q.pop_front();
        for (int i = pos.y - 1; i <= pos.y + 1; ++i) {
            for (int j = pos.x - 1; j <= pos.x + 1; ++j) {
                int c = int(i == pos.y) + int(j == pos.x);
                if (c != 1)
                    continue;
                if (isValid(j, i, map) && visited.find({j, i}) == visited.end()) {
                    q.push_back(std::make_pair(curr.first, Vector2i(j, i)));
                    visited.emplace(j, i);
                }
            }
        }
    }
    return (target - orig).normalize();
}

} // namespace

Direction getDir(const Vector2f & dir)
{
    float angle = std::atan2(dir.y, dir.x) * 180.f / M_PI;
    if (angle >= -22.5f && angle < 22.5f)
        return Direction::Right_Up;
    if (angle >= 22.5f && angle < 67.5f)
        return Direction::Right_Down;
    if (angle >= 67.5f && angle < 112.5f)
        return Direction::Down;
    if (angle >= 112.5f && angle < 157.5f)
        return Direction::Left_Down;
    if (angle >= 157.5f || angle < -157.5f)
        return Direction::Left;
    if (angle <= -112.5f && angle > -157.5f)
        return Direction::Up;
    if (angle <= -67.5f && angle > -112.5f)
        return Direction::Left_Up;
    return Direction::Right;
}

void Player::update(float gameSpeed, float dt)
{
    auto & resManager = iso::ResourceManager::getInstance();
    auto speed = gameSpeed * dt;
    move(dir * speed);

    reloadTimeLeft = std::max(0.f, reloadTimeLeft - dt);

    if (dir == Vector2f(-1, 0))
        setAnimation(resManager.getAnimation("mage-left"));
    else if (dir == Vector2f(-1, -1))
        setAnimation(resManager.getAnimation("mage-left-up"));
    else if (dir == Vector2f(0, -1))
        setAnimation(resManager.getAnimation("mage-up"));
    else if (dir == Vector2f(1, -1))
        setAnimation(resManager.getAnimation("mage-right-up"));
    else if (dir == Vector2f(1, 0))
        setAnimation(resManager.getAnimation("mage-right"));
    else if (dir == Vector2f(1, 1))
        setAnimation(resManager.getAnimation("mage-right-down"));
    else if (dir == Vector2f(0, 1))
        setAnimation(resManager.getAnimation("mage-down"));
    else if (dir == Vector2f(-1, 1))
        setAnimation(resManager.getAnimation("mage-left-down"));

    dir = {0, 0};
}

const Vector2f & Player::getFacingDir() const
{
    return facingDir;
}

void Player::faceDirection(const Vector2f & v)
{
    dir = v;
    facingDir = v;
}

bool Player::canShoot() const
{
    return reloadTimeLeft < std::numeric_limits<float>::epsilon();
}

void Player::shoot()
{
    reloadTimeLeft = reloadTime;
}

bool Player::collide(const GameObject * object)
{
    auto e = dynamic_cast<const Entity *>(object);
    if (e->type == Type::Portal) {
        portalCollide(e);
    }
    return true;
}

Zombie::Zombie(
    const Tile * map,
    Player * player,
    std::function<void()> playerReached)
    : Entity(Entity::Type::Zombie),
      map(map),
      player(player),
      playerReached(playerReached)
{
    auto & resManager = iso::ResourceManager::getInstance();
    dir = {-1, 0};
    animationDir = Direction::Left;
    setAnimation(resManager.getAnimation("zombie-left"));
}

void Zombie::update(float gameSpeed, float dt)
{
    auto & resManager = iso::ResourceManager::getInstance();

    moveTime += dt;
    if (moveTime < AnimationTime) {
        return;
    }

    moveTime -= AnimationTime;
    auto speed = gameSpeed * 0.1f;

    auto v = BFS(getPosition(), player->getPosition(), map);

    move(v * speed);
    auto newDir = getDir(v);

    if (newDir != animationDir) {
        animationDir = newDir;
        if (animationDir == Direction::Left)
            setAnimation(resManager.getAnimation("zombie-left"));
        else if (animationDir == Direction::Up)
            setAnimation(resManager.getAnimation("zombie-left-up"));
        else if (animationDir == Direction::Left_Up)
            setAnimation(resManager.getAnimation("zombie-up"));
        else if (animationDir == Direction::Right)
            setAnimation(resManager.getAnimation("zombie-right-up"));
        else if (animationDir == Direction::Right_Up)
            setAnimation(resManager.getAnimation("zombie-right"));
        else if (animationDir == Direction::Right_Down)
            setAnimation(resManager.getAnimation("zombie-right-down"));
        else if (animationDir == Direction::Down)
            setAnimation(resManager.getAnimation("zombie-down"));
        else
            setAnimation(resManager.getAnimation("zombie-left-down"));
    }
}

bool Zombie::collide(const GameObject * object)
{
    if (dynamic_cast<const Player *>(object)) {
        playerReached();
    }
    return true;
}

Fireball::Fireball(
    Vector2f dir,
    std::function<void(const Fireball *, const Zombie *)> fbCollide)
    : Entity(Entity::Type::Fireball), dir(dir), fbCollide(fbCollide)
{
    auto & resManager = iso::ResourceManager::getInstance();
    if (dir == Vector2f(-1, 0))
        setAnimation(resManager.getAnimation("fb-left"));
    else if (dir == Vector2f(-1, -1))
        setAnimation(resManager.getAnimation("fb-left-up"));
    else if (dir == Vector2f(0, -1))
        setAnimation(resManager.getAnimation("fb-up"));
    else if (dir == Vector2f(1, -1))
        setAnimation(resManager.getAnimation("fb-right-up"));
    else if (dir == Vector2f(1, 0))
        setAnimation(resManager.getAnimation("fb-right"));
    else if (dir == Vector2f(1, 1))
        setAnimation(resManager.getAnimation("fb-right-down"));
    else if (dir == Vector2f(0, 1))
        setAnimation(resManager.getAnimation("fb-down"));
    else if (dir == Vector2f(-1, 1))
        setAnimation(resManager.getAnimation("fb-left-down"));
    dir = dir.normalize();
}

void Fireball::update(float gameSpeed, float dt)
{
    auto speed = gameSpeed * dt * 8;
    move(dir * speed);
}

bool Fireball::collide(const GameObject * object)
{
    auto z = dynamic_cast<const Zombie *>(object);
    // Implicitly nullptr when it's not a zombie
    fbCollide(this, z);
    return true;
}

bool Fireball::collide()
{
    fbCollide(this, nullptr);
    return true;
}
