#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "isoengine/common/hashedstring.h"
#include <map>
#include <memory>
#include <queue>

namespace iso
{
class Engine;
class GameObject;
}

namespace iso::events
{

struct Command {
    HashedString type;
    Command(HashedString type) : type(type) {}
};

using SenderCommand = std::pair<GameObject *, std::shared_ptr<Command>>;

class CommandQueue
{
    friend class iso::Engine;

    struct HSComparator {
        bool operator()(const HashedString & lhs, const HashedString & rhs) const
        {
            return lhs.getHash() < rhs.getHash();
        }
    };

public:
    void pushCommand(GameObject * sender,
                     std::shared_ptr<Command> command);
    bool isEmpty() const { return queue.empty(); }
    unsigned generateCommandTypes(std::initializer_list<HashedString> types) const;

private:
    void registerCommand(HashedString command);
    SenderCommand popCommand();
    bool objectListensToCommand(const GameObject & object,
                                const Command & cmd) const;
    size_t size() { return queue.size(); }

    std::queue<SenderCommand> queue;
    std::map<HashedString, unsigned, HSComparator> commandIDs;
};

} // namespace iso

#endif // COMMANDQUEUE_H
