#include "isoengine/events/event.h"
#include <SFML/Window/Keyboard.hpp>

namespace iso::Keyboard
{

bool isKeyPressed(KeyCode key)
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Key(key));
}

} // namespace iso::Keyboard