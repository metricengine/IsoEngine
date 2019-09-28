#ifndef ENTITY_H
#define ENTITY_H

#include "isoengine/highlevel/gameobject.h"
#include <functional>

enum class Direction {
    Left,
    Left_Up,
    Up,
    Right_Up,
    Right,
    Right_Down,
    Down,
    Left_Down
};

Direction getDir(const iso::math::Vector2f & dir);

class Entity : public iso::GameObject
{
public:
    enum class Type {
        Zombie,
        Mage,
        Portal,
        Fireball,
        Tile
    };

    Entity() {}

    Entity(Type type) : type(type)
    {
    }

    Type type;
};

class Player : public Entity
{
public:
    Player(std::function<void(const Entity *)> portalFn)
        : Entity(Entity::Type::Mage),
          portalCollideFunction(portalFn)
    {
        facingDir = {-1, 0};
    }

    void update(float gameSpeed, float dt);
    const iso::Vector2f & getFacingDir() const;
    void faceDirection(const iso::Vector2f & v);

private:
    bool collide(const GameObject * object) override;

    std::function<void(const Entity *)> portalCollideFunction;
    iso::Vector2f dir, facingDir;
};

class Zombie : public Entity
{
public:
    Zombie(Player * player);

    void update(float gameSpeed, float dt);

private:
    Player * player;
    iso::Vector2f dir;
    Direction animationDir;
};

class Fireball : public Entity
{
public:
    Fireball(
        iso::math::Vector2f dir,
        std::function<void(const Fireball *, const Zombie *)> fbFn);
    void update(float gameSpeed, float dt);
    bool collide(const GameObject * object) override;
    bool collide() override;

private:
    std::function<void(const Fireball *, const Zombie *)> fireballCollideFn;
    iso::Vector2f dir;
};

#endif // ENTITY_H
