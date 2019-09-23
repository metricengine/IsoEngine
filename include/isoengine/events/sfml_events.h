#include "isoengine/events/event.h"

#ifndef SFML_EVENTS_H
#define SFML_EVENTS_H

namespace sf
{
class Event;
}

namespace iso
{

KeyEvent keyEventFromSFML(const sf::Event & event);
MouseEvent mouseEventFromSFML(const sf::Event & event);

} // namespace iso

#endif // SFML_EVENTS_H
