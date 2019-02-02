#ifndef WINDOW_H
#define WINDOW_H

#include "isoengine/math/transform.h"
#include "sprite.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

class Drawable;

namespace iso
{
class Window
{
private:
    sf::RenderWindow window;

public:
    explicit Window(const std::string & title);
    bool isOpen();
    void close();
    bool pollEvent(sf::Event & event);
    void setView(sf::View view);
    const sf::View & getView();
    void clear(sf::Color color);
    void display();
    void draw(const Sprite & sprite, math::Transform transform);
    sf::RenderWindow & getWindow()
    {
        return window;
    }
};
} // namespace iso

#endif
