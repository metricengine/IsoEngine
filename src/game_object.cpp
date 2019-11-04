#include "isoengine/game_object.h"
#include "isoengine/physics/collision_detector.h"

namespace iso
{

void GameObject::setCommandTypes(std::initializer_list<support::HashedString> types)
{
    if (commandQueue)
        commandTypes = commandQueue->generateCommandTypes(types);
}

void GameObject::sendCommand(std::shared_ptr<events::Command> command)
{
    if (commandQueue)
        commandQueue->pushCommand(this, command);
}

void GameObject::move(const math::Vector2f & dir, float speed)
{
    auto newPos = getPosition() + dir * speed;

    if (collisionDetector == nullptr ||
        collisionDetector->checkCollisions(this, newPos) == false) {
        setPosition(newPos);
    }
}

} // namespace iso
