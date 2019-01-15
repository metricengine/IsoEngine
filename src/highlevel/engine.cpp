#include "isoengine/highlevel/engine.h"
#include "isoengine/common/clock.h"
#include "isoengine/support/resourcemanager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace iso
{

Engine::Engine(std::initializer_list<HashedString> layerNames) : window{"IsoEngine"}, scene{layerNames}
{
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

void Engine::addEventHandler(EventHandler eventHandler)
{
    eventHandlers.push_back(eventHandler);
}

void Engine::registerCommand(HashedString command)
{
    commandQueue.registerCommand(command);
}

void Engine::addCommandHandler(CommandHandler cmdHandler)
{
    commandHandlers.push_back(cmdHandler);
}

void Engine::addGameObject(std::shared_ptr<GameObject> gameObject)
{
    scene.topLayer().addChild(gameObject);
    gameObjects.push_back(gameObject);
    gameObject->setCommandQueue(&commandQueue);
}

void Engine::addGameObject(std::shared_ptr<GameObject> gameObject, HashedString layerName)
{
    scene.getLayer(layerName).addChild(gameObject);
    gameObjects.push_back(gameObject);
    gameObject->setCommandQueue(&commandQueue);
}

void Engine::handleInput()
{
    Event engineEvent;
    sf::Event & event = engineEvent.event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            engineEvent.type = EventType::Mouse;

            auto sceneCoords = window.getWindow().mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

            // iso::Animator animator;
            // animator.getSprite().setPosition({sceneCoords.x, sceneCoords.y});
            // animator.setAnimation(iso::ResourceManager::getInstance().getAnimation("crystal"));
            // animator.setAnimationSpeed((rand() % 200) / 200.f + 0.5f);
            // animators.push_back(std::move(animator));
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

        // if (event.type == sf::Event::MouseMoved) {
        //     sf::View view = window.getView();
        //     view.setCenter(sf::Vector2f{sf::Mouse::getPosition()});
        //     window.setView(view);
        // }

        if (event.type == sf::Event::KeyPressed) {
            engineEvent.type = EventType::Key;
        }
    }
    for (auto handler : eventHandlers) {
        handler(engineEvent);
    }
}

void Engine::update(float dt)
{
    while (!commandQueue.isEmpty()) {
        auto cmd = commandQueue.popCommand();
        auto & sender = *cmd.first;
        auto & cmdType = *cmd.second.get();

        for (auto cmdHandler : commandHandlers)
            cmdHandler(sender, cmdType);
        for (auto gameObject : gameObjects)
            if (commandQueue.objectListensToCommand(*gameObject, cmdType))
                gameObject->handleCommand(sender, cmdType);
    }
    for (auto gameObject : gameObjects)
        gameObject->update(dt);
}

void Engine::render()
{
    window.clear(sf::Color::White);

    scene.draw(window);

    // for (auto & layer : layers) {
    //     window.draw(layer);
    // }

    // for (int i = 0; i < 2; ++i) {
    //     bgSprite.setPosition({i * 512.f, 0.f});
    //     window.draw(bgSprite);
    // }

    // for (auto & animator : animators)
    //     window.draw(animator.getSprite());
    // for (auto & gameObject : gameObjects)
    //     window.draw(gameObject->getSprite());

    window.display();
}

} // namespace iso