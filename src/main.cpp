#include "isoengine/render/sprite.h"
#include "isoengine/render/window.h"
#include "isoengine/support/resourcemanager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    iso::Window window("IsoEngine");

    iso::Texture texture{iso::ResourceManager::getInstance().getTexture("res/textures/dummy.png")};
    texture.setSmooth(true);
    iso::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize() / 2);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        window.clear(sf::Color::White);

        sprite.setPosition({sf::Mouse::getPosition(window.getWindow()).x, sf::Mouse::getPosition(window.getWindow()).y});
        window.draw(sprite);
        window.display();
    }

    return 0;
}
