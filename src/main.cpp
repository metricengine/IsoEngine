#include "isoengine/render/sprite.h"
#include "isoengine/support/resourcemanager.h"
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "IsoEngine");

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

        sprite.setPosition({sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y});
        window.draw(sprite.getSprite());
        window.display();
    }

    return 0;
}
