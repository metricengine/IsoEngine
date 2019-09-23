#include "isoengine/events/sfml_events.h"
#include <SFML/Window/Event.hpp>

namespace iso
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

    switch (event.key.code) {
    case sf::Keyboard::A:
        keyEvent.keyCode = KeyCode::A;
        break;
    case sf::Keyboard::B:
        keyEvent.keyCode = KeyCode::B;
        break;
    case sf::Keyboard::C:
        keyEvent.keyCode = KeyCode::C;
        break;
    case sf::Keyboard::D:
        keyEvent.keyCode = KeyCode::D;
        break;
    case sf::Keyboard::E:
        keyEvent.keyCode = KeyCode::E;
        break;
    case sf::Keyboard::F:
        keyEvent.keyCode = KeyCode::F;
        break;
    case sf::Keyboard::G:
        keyEvent.keyCode = KeyCode::G;
        break;
    case sf::Keyboard::H:
        keyEvent.keyCode = KeyCode::H;
        break;
    case sf::Keyboard::I:
        keyEvent.keyCode = KeyCode::I;
        break;
    case sf::Keyboard::J:
        keyEvent.keyCode = KeyCode::J;
        break;
    case sf::Keyboard::K:
        keyEvent.keyCode = KeyCode::K;
        break;
    case sf::Keyboard::L:
        keyEvent.keyCode = KeyCode::L;
        break;
    case sf::Keyboard::M:
        keyEvent.keyCode = KeyCode::M;
        break;
    case sf::Keyboard::N:
        keyEvent.keyCode = KeyCode::N;
        break;
    case sf::Keyboard::O:
        keyEvent.keyCode = KeyCode::O;
        break;
    case sf::Keyboard::P:
        keyEvent.keyCode = KeyCode::P;
        break;
    case sf::Keyboard::Q:
        keyEvent.keyCode = KeyCode::Q;
        break;
    case sf::Keyboard::R:
        keyEvent.keyCode = KeyCode::R;
        break;
    case sf::Keyboard::S:
        keyEvent.keyCode = KeyCode::S;
        break;
    case sf::Keyboard::T:
        keyEvent.keyCode = KeyCode::T;
        break;
    case sf::Keyboard::U:
        keyEvent.keyCode = KeyCode::U;
        break;
    case sf::Keyboard::V:
        keyEvent.keyCode = KeyCode::V;
        break;
    case sf::Keyboard::W:
        keyEvent.keyCode = KeyCode::W;
        break;
    case sf::Keyboard::X:
        keyEvent.keyCode = KeyCode::X;
        break;
    case sf::Keyboard::Y:
        keyEvent.keyCode = KeyCode::Y;
        break;
    case sf::Keyboard::Z:
        keyEvent.keyCode = KeyCode::Z;
        break;
    case sf::Keyboard::Num0:
        keyEvent.keyCode = KeyCode::Num0;
        break;
    case sf::Keyboard::Num1:
        keyEvent.keyCode = KeyCode::Num1;
        break;
    case sf::Keyboard::Num2:
        keyEvent.keyCode = KeyCode::Num2;
        break;
    case sf::Keyboard::Num3:
        keyEvent.keyCode = KeyCode::Num3;
        break;
    case sf::Keyboard::Num4:
        keyEvent.keyCode = KeyCode::Num4;
        break;
    case sf::Keyboard::Num5:
        keyEvent.keyCode = KeyCode::Num5;
        break;
    case sf::Keyboard::Num6:
        keyEvent.keyCode = KeyCode::Num6;
        break;
    case sf::Keyboard::Num7:
        keyEvent.keyCode = KeyCode::Num7;
        break;
    case sf::Keyboard::Num8:
        keyEvent.keyCode = KeyCode::Num8;
        break;
    case sf::Keyboard::Num9:
        keyEvent.keyCode = KeyCode::Num9;
        break;
    case sf::Keyboard::Escape:
        keyEvent.keyCode = KeyCode::Escape;
        break;
    case sf::Keyboard::LControl:
        keyEvent.keyCode = KeyCode::LControl;
        break;
    case sf::Keyboard::LShift:
        keyEvent.keyCode = KeyCode::LShift;
        break;
    case sf::Keyboard::LAlt:
        keyEvent.keyCode = KeyCode::LAlt;
        break;
    case sf::Keyboard::LSystem:
        keyEvent.keyCode = KeyCode::LSystem;
        break;
    case sf::Keyboard::RControl:
        keyEvent.keyCode = KeyCode::RControl;
        break;
    case sf::Keyboard::RShift:
        keyEvent.keyCode = KeyCode::RShift;
        break;
    case sf::Keyboard::RAlt:
        keyEvent.keyCode = KeyCode::RAlt;
        break;
    case sf::Keyboard::RSystem:
        keyEvent.keyCode = KeyCode::RSystem;
        break;
    case sf::Keyboard::Menu:
        keyEvent.keyCode = KeyCode::Menu;
        break;
    case sf::Keyboard::LBracket:
        keyEvent.keyCode = KeyCode::LBracket;
        break;
    case sf::Keyboard::RBracket:
        keyEvent.keyCode = KeyCode::RBracket;
        break;
    case sf::Keyboard::Semicolon:
        keyEvent.keyCode = KeyCode::Semicolon;
        break;
    case sf::Keyboard::Comma:
        keyEvent.keyCode = KeyCode::Comma;
        break;
    case sf::Keyboard::Period:
        keyEvent.keyCode = KeyCode::Period;
        break;
    case sf::Keyboard::Quote:
        keyEvent.keyCode = KeyCode::Quote;
        break;
    case sf::Keyboard::Slash:
        keyEvent.keyCode = KeyCode::Slash;
        break;
    case sf::Keyboard::Backslash:
        keyEvent.keyCode = KeyCode::Backslash;
        break;
    case sf::Keyboard::Tilde:
        keyEvent.keyCode = KeyCode::Tilde;
        break;
    case sf::Keyboard::Equal:
        keyEvent.keyCode = KeyCode::Equal;
        break;
    case sf::Keyboard::Hyphen:
        keyEvent.keyCode = KeyCode::Hyphen;
        break;
    case sf::Keyboard::Space:
        keyEvent.keyCode = KeyCode::Space;
        break;
    case sf::Keyboard::Enter:
        keyEvent.keyCode = KeyCode::Enter;
        break;
    case sf::Keyboard::Backspace:
        keyEvent.keyCode = KeyCode::Backspace;
        break;
    case sf::Keyboard::Tab:
        keyEvent.keyCode = KeyCode::Tab;
        break;
    case sf::Keyboard::PageUp:
        keyEvent.keyCode = KeyCode::PageUp;
        break;
    case sf::Keyboard::PageDown:
        keyEvent.keyCode = KeyCode::PageDown;
        break;
    case sf::Keyboard::End:
        keyEvent.keyCode = KeyCode::End;
        break;
    case sf::Keyboard::Home:
        keyEvent.keyCode = KeyCode::Home;
        break;
    case sf::Keyboard::Insert:
        keyEvent.keyCode = KeyCode::Insert;
        break;
    case sf::Keyboard::Delete:
        keyEvent.keyCode = KeyCode::Delete;
        break;
    case sf::Keyboard::Add:
        keyEvent.keyCode = KeyCode::Add;
        break;
    case sf::Keyboard::Subtract:
        keyEvent.keyCode = KeyCode::Subtract;
        break;
    case sf::Keyboard::Multiply:
        keyEvent.keyCode = KeyCode::Multiply;
        break;
    case sf::Keyboard::Divide:
        keyEvent.keyCode = KeyCode::Divide;
        break;
    case sf::Keyboard::Left:
        keyEvent.keyCode = KeyCode::Left;
        break;
    case sf::Keyboard::Right:
        keyEvent.keyCode = KeyCode::Right;
        break;
    case sf::Keyboard::Up:
        keyEvent.keyCode = KeyCode::Up;
        break;
    case sf::Keyboard::Down:
        keyEvent.keyCode = KeyCode::Down;
        break;
    case sf::Keyboard::Numpad0:
        keyEvent.keyCode = KeyCode::Numpad0;
        break;
    case sf::Keyboard::Numpad1:
        keyEvent.keyCode = KeyCode::Numpad1;
        break;
    case sf::Keyboard::Numpad2:
        keyEvent.keyCode = KeyCode::Numpad2;
        break;
    case sf::Keyboard::Numpad3:
        keyEvent.keyCode = KeyCode::Numpad3;
        break;
    case sf::Keyboard::Numpad4:
        keyEvent.keyCode = KeyCode::Numpad4;
        break;
    case sf::Keyboard::Numpad5:
        keyEvent.keyCode = KeyCode::Numpad5;
        break;
    case sf::Keyboard::Numpad6:
        keyEvent.keyCode = KeyCode::Numpad6;
        break;
    case sf::Keyboard::Numpad7:
        keyEvent.keyCode = KeyCode::Numpad7;
        break;
    case sf::Keyboard::Numpad8:
        keyEvent.keyCode = KeyCode::Numpad8;
        break;
    case sf::Keyboard::Numpad9:
        keyEvent.keyCode = KeyCode::Numpad9;
        break;
    case sf::Keyboard::F1:
        keyEvent.keyCode = KeyCode::F1;
        break;
    case sf::Keyboard::F2:
        keyEvent.keyCode = KeyCode::F2;
        break;
    case sf::Keyboard::F3:
        keyEvent.keyCode = KeyCode::F3;
        break;
    case sf::Keyboard::F4:
        keyEvent.keyCode = KeyCode::F4;
        break;
    case sf::Keyboard::F5:
        keyEvent.keyCode = KeyCode::F5;
        break;
    case sf::Keyboard::F6:
        keyEvent.keyCode = KeyCode::F6;
        break;
    case sf::Keyboard::F7:
        keyEvent.keyCode = KeyCode::F7;
        break;
    case sf::Keyboard::F8:
        keyEvent.keyCode = KeyCode::F8;
        break;
    case sf::Keyboard::F9:
        keyEvent.keyCode = KeyCode::F9;
        break;
    case sf::Keyboard::F10:
        keyEvent.keyCode = KeyCode::F10;
        break;
    case sf::Keyboard::F11:
        keyEvent.keyCode = KeyCode::F11;
        break;
    case sf::Keyboard::F12:
        keyEvent.keyCode = KeyCode::F12;
        break;
    case sf::Keyboard::F13:
        keyEvent.keyCode = KeyCode::F13;
        break;
    case sf::Keyboard::F14:
        keyEvent.keyCode = KeyCode::F14;
        break;
    case sf::Keyboard::F15:
        keyEvent.keyCode = KeyCode::F15;
        break;
    case sf::Keyboard::Pause:
        keyEvent.keyCode = KeyCode::Pause;
        break;
    case sf::Keyboard::KeyCount:
        keyEvent.keyCode = KeyCode::KeyCount;
        break;
    default:
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
