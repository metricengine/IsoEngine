#ifndef EVENT_H
#define EVENT_H

#include <SFML/Window.hpp>

namespace iso
{

enum class EventType {
    Key,
    Mouse,
    Invalid
};

struct Event {
    sf::Event event;
    EventType type;
};

} // namespace iso

#endif // EVENT_H
