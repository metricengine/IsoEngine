#include "entity.h"
#include "game.h"
#include "isoengine/support/resource_manager.h"
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

Player::Player(std::function<void(const Entity *)> portalCollide)
    : Entity(Entity::Type::Mage), portalCollide(portalCollide)
{
    auto & resManager = iso::support::ResourceManager::getInstance();
    facingDir = {-1, 0};
    setAnimations({resManager.getAnimation("mage-right"),
                   resManager.getAnimation("mage-right-up"),
                   resManager.getAnimation("mage-up"),
                   resManager.getAnimation("mage-left-up"),
                   resManager.getAnimation("mage-left"),
                   resManager.getAnimation("mage-left-down"),
                   resManager.getAnimation("mage-down"),
                   resManager.getAnimation("mage-right-down")});
    move(facingDir, 0.f);
}

void Player::update(float gameSpeed, float dt)
{
    auto speed = gameSpeed * dt;
    if (dir.x != 0 || dir.y != 0) {
        move(dir, speed);
    }

    reloadTimeLeft = std::max(0.f, reloadTimeLeft - dt);
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
    auto & resManager = iso::support::ResourceManager::getInstance();
    setAnimations({resManager.getAnimation("zombie-right"),
                   resManager.getAnimation("zombie-right-up"),
                   resManager.getAnimation("zombie-up"),
                   resManager.getAnimation("zombie-left-up"),
                   resManager.getAnimation("zombie-left"),
                   resManager.getAnimation("zombie-left-down"),
                   resManager.getAnimation("zombie-down"),
                   resManager.getAnimation("zombie-right-down")});
    move(Vector2f(-1.f, 0.f), 0.f);
}

void Zombie::update(float gameSpeed, float dt)
{
    moveTime += dt;
    if (moveTime < AnimationTime) {
        return;
    }

    moveTime -= AnimationTime;
    auto speed = gameSpeed * 0.1f;

    auto v = BFS(getPosition(), player->getPosition(), map);
    move(v, speed);
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
    : Entity(Entity::Type::Fireball), fbCollide(fbCollide)
{
    auto & resManager = iso::support::ResourceManager::getInstance();
    setAnimations({resManager.getAnimation("fb-right"),
                   resManager.getAnimation("fb-right-up"),
                   resManager.getAnimation("fb-up"),
                   resManager.getAnimation("fb-left-up"),
                   resManager.getAnimation("fb-left"),
                   resManager.getAnimation("fb-left-down"),
                   resManager.getAnimation("fb-down"),
                   resManager.getAnimation("fb-right-down")});
    move(dir, 0.f);
}

void Fireball::update(float gameSpeed, float dt)
{
    auto speed = gameSpeed * dt * 8;
    move(facingDirection(), speed);
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
