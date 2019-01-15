#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "isoengine/common/hashedstring.h"
#include "isoengine/events/commandqueue.h"
#include "isoengine/render/animator.h"
#include "renderscene.h"
#include <initializer_list>

#include <iostream>

namespace iso
{

class GameObject : public Animator, public SceneNode
{
    friend class CommandQueue;
    friend class Engine;

public:
    void setCommandTypes(std::initializer_list<HashedString> types);
    void sendCommand(std::shared_ptr<Command> command);

    math::Vector2f getPosition()
    {
        return getSprite().getPosition();
    }

    void setPosition(const math::Vector2f & pos)
    {
        getSprite().setPosition(pos);
    }

    void move(const math::Vector2f & dir)
    {
        getSprite().setPosition(getPosition() + dir);
    }

private:
    virtual void handleCommand(GameObject & sender,
                               const Command & command) {}

    void drawCurrent(Window & window) const override
    {
        // std::cout << "drawCurrent gameobject" << std::endl;
        window.draw(getSprite());
    }

    void setCommandQueue(CommandQueue * cq)
    {
        commandQueue = cq;
    }

    unsigned commandTypes = 0;
    CommandQueue * commandQueue = nullptr;
};

} // namespace iso

#endif // GAMEOBJECT_H
