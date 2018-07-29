#include "isoengine/highlevel/engine.h"
#include "isoengine/common/clock.h"
#include "isoengine/support/resourcemanager.h"
#include <SFML/Graphics.hpp>

#include <iostream>

namespace iso
{

Engine::Engine() : window{"IsoEngine"}
{
    iso::Texture & texture = iso::ResourceManager::getInstance().getTexture("res/textures/dummy.png");
    iso::Texture & textureCrystal = iso::ResourceManager::getInstance().getTexture("res/textures/crystal.png");
    iso::ResourceManager::getInstance().addAnimation("crystal", iso::Animation(textureCrystal, iso::math::Vector2i(0, 0), iso::math::Vector2i(32, 32), 8, 1, true));
    texture.setSmooth(true);

    bgSprite.setTexture(texture);
    bgSprite.setScale(0.5, 0.5);
}

void Engine::run()
{
    Clock<std::chrono::microseconds> clock;
    float dt{};

    while (window.getWindow().isOpen()) {
        handleInput();

        dt += clock.restart() / 1000000.f;

        while (dt >= timePerFrame) {
            dt -= timePerFrame;
            update(timePerFrame);
        }

        render();
    }
}

void Engine::handleInput()
{
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            auto sceneCoords = window.getWindow().mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

            iso::Animator animator;
            animator.getSprite().setPosition({sceneCoords.x, sceneCoords.y});
            animator.setAnimation(iso::ResourceManager::getInstance().getAnimation("crystal"));
            animator.setAnimationSpeed((rand() % 200) / 200.f + 0.5f);
            animators.push_back(std::move(animator));
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
}

void Engine::update(float dt)
{
    for (auto & animator : animators)
        animator.update(dt);
}

void Engine::render()
{
    window.clear(sf::Color::White);

    for (int i = 0; i < 2; ++i) {
        bgSprite.setPosition({i * 512.f, 0.f});
        window.draw(bgSprite);
    }

    for (auto & animator : animators)
        window.draw(animator.getSprite());

    window.display();
}

} // namespace iso