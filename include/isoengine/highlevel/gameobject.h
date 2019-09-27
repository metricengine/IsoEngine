#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "isoengine/common/hashedstring.h"
#include "isoengine/events/commandqueue.h"
#include "isoengine/math/transform.h"
#include "isoengine/render/animator.h"
#include "renderscene.h"
#include <initializer_list>

namespace iso
{

class CollisionDetector;

class GameObject : public Animator, public SceneNode
{
    friend class CommandQueue;
    friend class Engine;
    friend class CollisionDetector;

public:
    void setCommandTypes(std::initializer_list<HashedString> types);
    void sendCommand(std::shared_ptr<Command> command);

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
                               const Command & command) {}
    // override collide function to handle manual collision logic
    // return value -> object is colliding with argument, therefore cannot move
    virtual bool collide(const GameObject & object) { return true; }

    void drawCurrent(Window & window, Transform transform) const override
    {
        window.draw(getSprite(), transform);
    }

    void setCommandQueue(CommandQueue * cq)
    {
        commandQueue = cq;
    }

    void setCollisionDetector(CollisionDetector * cd)
    {
        collisionDetector = cd;
    }

    unsigned commandTypes = 0;
    CommandQueue * commandQueue = nullptr;
    CollisionDetector * collisionDetector = nullptr;
};

} // namespace iso

#endif // GAMEOBJECT_H
