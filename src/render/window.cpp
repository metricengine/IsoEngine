#include "isoengine/render/window.h"
#include <SFML/Window/Event.hpp>

namespace iso
{
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

void Window::draw(const Sprite & sprite, sf::Transform transform)
{
    sf::RenderStates states;
    states.transform = transform;
    window.draw(sprite.getSprite(), states);
}

} // namespace iso
