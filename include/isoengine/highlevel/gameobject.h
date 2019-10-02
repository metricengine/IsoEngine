#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "isoengine/support/hashed_string.h"
#include "isoengine/events/command_queue.h"
#include "isoengine/math/transform.h"
#include "isoengine/render/animator.h"
#include "isoengine/render/scene_node.h"
#include <initializer_list>

namespace iso::physics
{

class CollisionDetector;

}

namespace iso
{

class GameObject : public render::Animator, public render::SceneNode
{
    friend class events::CommandQueue;
    friend class Engine;
    friend class physics::CollisionDetector;

public:
    void setCommandTypes(std::initializer_list<support::HashedString> types);
    void sendCommand(std::shared_ptr<events::Command> command);

    // const math::Vector2f & getPosition() const
    // {
    //     return getSprite().getPosition();
    // }

    // void setPosition(const math::Vector2f & pos)
    // {
    //     getSprite().setPosition(pos);
    // }

    void move(const math::Vector2f & dir);

private:
    virtual void handleCommand(GameObject & sender,
                               const events::Command & command) {}
    // override collide function to handle manual collision logic
    // return value -> object is colliding with argument, therefore cannot move
    virtual bool collide(const GameObject * object) { return true; }
    // overload for colliding against global bounds
    virtual bool collide() { return true; }

    void drawCurrent(render::Window & window, math::Transform transform) const override
    {
        window.draw(getSprite(), transform);
    }

    void setCommandQueue(events::CommandQueue * cq)
    {
        commandQueue = cq;
    }

    void setCollisionDetector(physics::CollisionDetector * cd)
    {
        collisionDetector = cd;
    }

    unsigned commandTypes = 0;
    events::CommandQueue * commandQueue = nullptr;
    physics::CollisionDetector * collisionDetector = nullptr;
    math::Rectf boundingBox;
};

} // namespace iso

#endif // GAMEOBJECT_H
