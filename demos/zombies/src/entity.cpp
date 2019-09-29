#include "entity.h"
#include "isoengine/support/resourcemanager.h"

Direction getDir(const iso::math::Vector2f & dir)
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
    auto speed = gameSpeed * dt * 4;
    move(dir * speed);

    if (dir == iso::math::Vector2f(-1, 0))
        setAnimation(resManager.getAnimation("mage-left"));
    else if (dir == iso::math::Vector2f(0, -1))
        setAnimation(resManager.getAnimation("mage-up"));
    else if (dir == iso::math::Vector2f(1, 0))
        setAnimation(resManager.getAnimation("mage-right"));
    else if (dir == iso::math::Vector2f(0, 1))
        setAnimation(resManager.getAnimation("mage-down"));

    dir = {0, 0};
}

const iso::Vector2f & Player::getFacingDir() const
{
    return facingDir;
}

void Player::faceDirection(const iso::Vector2f & v)
{
    dir = v;
    facingDir = v;
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
    Player * player,
    std::function<void()> playerReached)
    : Entity(Entity::Type::Zombie),
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
    auto speed = gameSpeed * dt;
    auto playerCenter = player->getPosition();
    auto v = (playerCenter - getPosition()).normalize();
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
    if (auto player = dynamic_cast<const Player *>(object)) {
        playerReached();
    }
    return true;
}

Fireball::Fireball(
    iso::math::Vector2f dir,
    std::function<void(const Fireball *, const Zombie *)> fbCollide)
    : Entity(Entity::Type::Fireball), dir(dir), fbCollide(fbCollide)
{
    auto & resManager = iso::ResourceManager::getInstance();
    if (dir == iso::math::Vector2f(-1, 0))
        setAnimation(resManager.getAnimation("fb-left"));
    else if (dir == iso::math::Vector2f(0, -1))
        setAnimation(resManager.getAnimation("fb-up"));
    else if (dir == iso::math::Vector2f(1, 0))
        setAnimation(resManager.getAnimation("fb-right"));
    else if (dir == iso::math::Vector2f(0, 1))
        setAnimation(resManager.getAnimation("fb-down"));
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
