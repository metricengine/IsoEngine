#ifndef ENTITY_H
#define ENTITY_H

#include "isoengine/highlevel/gameobject.h"
#include <functional>

class Entity : public iso::GameObject
{
public:
    enum class Type {
        Zombie,
        Mage,
        Portal,
        Tile
    };

    Entity() {}

    Entity(Type type) : type(type)
    {
    }

    Type type;
};

class PlayerEntity : public Entity
{
public:
    PlayerEntity(std::function<void(const Entity *)> portalFn) : Entity(Entity::Type::Mage), portalCollideFunction(portalFn)
    {
    }

private:
    bool collide(const GameObject * object) override
    {
        auto e = dynamic_cast<const Entity *>(object);
        if (e->type == Type::Portal) {
            portalCollideFunction(e);
        }
        return true;
    }

    std::function<void(const Entity *)> portalCollideFunction;
};

#endif // ENTITY_H
