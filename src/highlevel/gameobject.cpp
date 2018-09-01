#include "isoengine/highlevel/gameobject.h"

namespace iso
{

void GameObject::setCommandTypes(std::initializer_list<HashedString> types)
{
    if (commandQueue)
        commandTypes = commandQueue->generateCommandTypes(types);
}

void GameObject::sendCommand(std::shared_ptr<Command> command)
{
    if (commandQueue)
        commandQueue->pushCommand(this, command);
}

} // namespace iso