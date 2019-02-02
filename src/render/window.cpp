#include "isoengine/render/window.h"
#include <SFML/Window/Event.hpp>

namespace iso
{

sf::Transform toSfmlTransform(const math::Transform & transform)
{
    const auto & m = transform.getMatrix();
    return sf::Transform(m(0, 0), m(0, 1), m(0, 2),
                         m(1, 0), m(1, 1), m(1, 2),
                         m(2, 0), m(2, 1), m(2, 2));
}

Window::Window(const std::string & title) : window(sf::VideoMode(800, 600), title)
{
}

bool Window::isOpen()
{
    return window.isOpen();
}

void Window::clear(sf::Color color)
{
    window.clear(color);
}

void Window::close()
{
    window.close();
}

bool Window::pollEvent(sf::Event & event)
{
    return window.pollEvent(event);
}

void Window::setView(sf::View view)
{
    window.setView(view);
}

const sf::View & Window::getView()
{
    return window.getView();
}

void Window::display()
{
    window.display();
}

void Window::draw(const Sprite & sprite, math::Transform transform)
{
    sf::RenderStates states;
    states.transform = toSfmlTransform(transform);
    window.draw(sprite.getSprite(), states);
}

} // namespace iso
