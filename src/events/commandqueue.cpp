#include "isoengine/events/commandqueue.h"
#include "isoengine/common/utils.h"
#include "isoengine/highlevel/gameobject.h"
#include <iostream>

namespace iso::events
{

void CommandQueue::pushCommand(GameObject * sender,
                               std::shared_ptr<Command> command)
{
    ASSERT(commandIDs.find(command->type) != commandIDs.end(),
           stringify("The command type was not registered: ", command->type.getCStr()).c_str());
    queue.push(std::make_pair(sender, command));
}

unsigned CommandQueue::generateCommandTypes(std::initializer_list<HashedString> types) const
{
    unsigned value = 0;
    for (HashedString type : types) {
        auto it = commandIDs.find(type);
        // ASSERT(it != commandIDs.end(),
        //        stringify("The command type was not registered: ", it->first.getCStr()).c_str());
        value |= it->second;
    }
    return value;
}

void CommandQueue::registerCommand(HashedString command)
{
    unsigned commandValue = 1 << commandIDs.size();
    commandIDs.insert(std::make_pair(command, commandValue));
}

SenderCommand CommandQueue::popCommand()
{
    auto cmd = queue.front();
    queue.pop();
    return cmd;
}

bool CommandQueue::objectListensToCommand(const GameObject & object,
                                          const Command & cmd) const
{
    auto it = commandIDs.find(cmd.type);
    ASSERT(it != commandIDs.end(), stringify("The command type was not registered: ", it->first.getCStr()).c_str());
    return it->second & object.commandTypes;
}

} // namespace iso
