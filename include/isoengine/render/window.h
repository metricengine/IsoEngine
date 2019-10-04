#ifndef WINDOW_H
#define WINDOW_H

#include "isoengine/math/transform.h"
#include "isoengine/render/sprite.h"
#include "isoengine/render/text.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

enum class WindowStyle {
    Resize,
    Static,
    Fullscreen
};

namespace iso::render
{

class Window
{
public:
    Window(const std::string & title, const iso::math::Vector2u & size, WindowStyle style);
    iso::math::Vector2u getSize() const;
    void setSize(const iso::math::Vector2u & size);
    bool isOpen() const;
    void close();
    bool pollEvent(sf::Event & event);
    void setView(sf::View view);
    const sf::View & getView();
    void clear(sf::Color color);
    void display();
    void draw(const Sprite & sprite, iso::math::Transform transform);
    void draw(const Text & text, iso::math::Transform transform);
    sf::RenderWindow & getWindow()
    {
        return window;
    }

private:
    sf::RenderWindow window;
    sf::Font font;
};
} // namespace iso

#endif
