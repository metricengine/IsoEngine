#include "isoengine/events/sfml_events.h"
#include <SFML/Window/Event.hpp>

namespace iso::events
{

KeyEvent keyEventFromSFML(const sf::Event & event)
{
    KeyEvent keyEvent;

    switch (event.type) {
    case sf::Event::KeyPressed:
        keyEvent.eventType = KeyEventType::KeyPressed;
        break;
    case sf::Event::KeyReleased:
        keyEvent.eventType = KeyEventType::KeyReleased;
        break;
    default:
        keyEvent.eventType = KeyEventType::Unknown;
    }

    if (event.key.code < sf::Keyboard::KeyCount) {
        keyEvent.keyCode = KeyCode(event.key.code);
    } else {
        keyEvent.keyCode = KeyCode::Unknown;
    }

    return keyEvent;
}

MouseEvent mouseEventFromSFML(const sf::Event & event)
{
    MouseEvent mouseEvent;

    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        mouseEvent.eventType = MouseEventType::MouseButtonPressed;
        mouseEvent.x = event.mouseButton.x;
        mouseEvent.y = event.mouseButton.y;
        break;
    case sf::Event::MouseButtonReleased:
        mouseEvent.eventType = MouseEventType::MouseButtonReleased;
        mouseEvent.x = event.mouseButton.x;
        mouseEvent.y = event.mouseButton.y;
        break;
    case sf::Event::MouseMoved:
        mouseEvent.eventType = MouseEventType::MouseMoved;
        mouseEvent.x = event.mouseMove.x;
        mouseEvent.y = event.mouseMove.y;
        break;
    default:
        mouseEvent.eventType = MouseEventType::Unknown;
    }

    return mouseEvent;
}

} // namespace iso
