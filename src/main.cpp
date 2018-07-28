#include "isoengine/render/animator.h"
#include "isoengine/render/sprite.h"
#include "isoengine/render/window.h"
#include "isoengine/support/resourcemanager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    iso::Window window("IsoEngine");
    window.getWindow().setFramerateLimit(60);

    iso::Texture texture{iso::ResourceManager::getInstance().getTexture("res/textures/dummy.png")};
    iso::Texture textureCrystal(iso::ResourceManager::getInstance().getTexture("res/textures/crystal.png"));
    iso::ResourceManager::getInstance().addAnimation("crystal", iso::Animation(textureCrystal, iso::math::Vector2i(0, 0), iso::math::Vector2i(32, 32), 8, 1, true));

    texture.setSmooth(true);
    iso::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.5, 0.5);

    std::vector<iso::Animator> animators;

    // float zoom = 1;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                iso::Animator animator;
                animator.getSprite().setPosition({(float)event.mouseButton.x, (float)event.mouseButton.y});
                animator.setAnimation(iso::ResourceManager::getInstance().getAnimation("crystal"));
                animator.setAnimationSpeed((rand() % 200) / 200.f + 0.5f);
                animators.push_back(std::move(animator));
            }

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            // if (event.type == sf::Event::MouseWheelScrolled) {
            //     sf::View view = window.getView();
            //     if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
            //         if (event.mouseWheelScroll.delta == 1) {
            //             zoom *= 0.9;
            //             view.zoom(zoom);
            //             window.setView(view);
            //         } else if (event.mouseWheelScroll.delta == -1) {
            //             zoom *= 1.1;
            //             view.zoom(zoom);
            //             window.setView(view);
            //         }
            //     }
            // }

            // if (event.type == sf::Event::MouseMoved) {
            //     sf::View view = window.getView();
            //     view.setCenter(sf::Vector2f{sf::Mouse::getPosition()});
            //     window.setView(view);
            // }
        }

        sf::Time dt = clock.restart();
        window.clear(sf::Color::White);

        // Update
        for (auto & animator : animators)
            animator.update(dt.asSeconds());

        for (int i = 0; i < 2; ++i) {
            sprite.setPosition({i * 512.f, 0.f});
            window.draw(sprite);
        }

        for (auto & animator : animators)
            window.draw(animator.getSprite());

        // Render
        window.display();
    }

    return 0;
}
