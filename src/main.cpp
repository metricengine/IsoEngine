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
    sprite.setScale(0.5, 0.5);

    float zoom = 1;

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

            if (event.type == sf::Event::MouseWheelScrolled) {
                sf::View view = window.getView();
                if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                    if (event.mouseWheelScroll.delta == 1) {
                        zoom *= 0.9;
                        view.zoom(zoom);
                        window.setView(view);
                    } else if (event.mouseWheelScroll.delta == -1) {
                        zoom *= 1.1;
                        view.zoom(zoom);
                        window.setView(view);
                    }
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::View view = window.getView();
                view.setCenter(sf::Vector2f{sf::Mouse::getPosition()});
                window.setView(view);
            }
        }

        window.clear(sf::Color::White);

        for (int i = 0; i < 2; ++i) {
            sprite.setPosition({i * 512, 0});
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}
