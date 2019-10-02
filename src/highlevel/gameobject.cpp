#include "isoengine/highlevel/gameobject.h"
#include "isoengine/physics/collision_detector.h"

namespace iso
{

void GameObject::setCommandTypes(std::initializer_list<HashedString> types)
{
    if (commandQueue)
        commandTypes = commandQueue->generateCommandTypes(types);
}

void GameObject::sendCommand(std::shared_ptr<events::Command> command)
{
    if (commandQueue)
        commandQueue->pushCommand(this, command);
}

void GameObject::move(const math::Vector2f & dir)
{
    auto newPos = getPosition() + dir;

    if (collisionDetector == nullptr ||
        collisionDetector->checkCollisions(this, newPos) == false) {
        setPosition(newPos);
    }
}

} // namespace iso
