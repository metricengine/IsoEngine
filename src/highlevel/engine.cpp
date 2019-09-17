#include "isoengine/highlevel/engine.h"
#include "isoengine/common/clock.h"
#include "isoengine/support/resourcemanager.h"
#include <SFML/Graphics.hpp>
#include <exception>

namespace iso
{

Engine::Engine(const WindowOptions & windowOpts, std::initializer_list<HashedString> layerNames)
    : scene{layerNames}
{
    switch (windowOpts.resizeStrategy) {
    case ResizeStrategy::FIXED_RES_STATIC_WINDOW:
        window = std::make_unique<Window>("IsoEngine", windowOpts.resolution, WindowStyle::Static);
        break;
    case ResizeStrategy::FIXED_RES_STRETCH:
    case ResizeStrategy::DYNAMIC_RES:
    // case ResizeStrategy::FIXED_ASPECT_RATIO:
    case ResizeStrategy::FIXED_ASPECT_RATIO_EXPAND_RES:
        window = std::make_unique<Window>("IsoEngine", windowOpts.resolution, WindowStyle::Resize);
        break;
    }
    resizeStrategy = windowOpts.resizeStrategy;
    auto center = window->getView().getCenter();
    camera.res = windowOpts.resolution;
    camera.pos = {center.x, center.y};
    camera.aspectRatio = windowOpts.aspectRatio;
    if (windowOpts.resizeStrategy == ResizeStrategy::FIXED_ASPECT_RATIO_EXPAND_RES && windowOpts.aspectRatio == math::Vector2u(0, 0)) {
        throw std::invalid_argument("Aspect ratio has to be provided if a FIXED_ASPECT_RATIO resize strategy is used");
    }
}

void Engine::setGameLoop(GameLoop callback)
{
    gameLoop = callback;
}

void Engine::run()
{
    Clock<std::chrono::microseconds> clock;
    float dt{};

    while (window->getWindow().isOpen()) {
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

void Engine::moveCamera(Vector2f dir)
{
    if (camera.following) {
        return;
    }
    // TODO: check boundries
    camera.pos += dir;
}

void Engine::zoomCamera(float scale)
{
    // TODO: reasonable constaints
    camera.zoom *= scale;
}

void Engine::cameraFollowObject(const GameObject * obj)
{
    camera.following = obj;
}

void Engine::cameraStopFollowing()
{
    camera.following = nullptr;
}

math::Vector2f Engine::screenToWorldCoords(const math::Vector2i & coords)
{
    auto res = window->getWindow().mapPixelToCoords(sf::Vector2i(coords.x, coords.y));
    return {res.x, res.y};
}

void Engine::addGameObject(std::shared_ptr<GameObject> gameObject, HashedString layerName)
{
    scene.getLayer(layerName).addChild(gameObject);
    gameObjects.push_back(gameObject);
    gameObject->setCommandQueue(&commandQueue);
}

void Engine::registerGameObject(std::shared_ptr<GameObject> gameObject)
{
    gameObjects.push_back(gameObject);
    gameObject->setCommandQueue(&commandQueue);
}

void Engine::handleInput()
{
    Event engineEvent;
    sf::Event & event = engineEvent.event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            engineEvent.type = EventType::Mouse;
        }

        if (event.type == sf::Event::Resized) {
            if (resizeStrategy == ResizeStrategy::DYNAMIC_RES) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window->setView(sf::View(visibleArea));
            } else if (resizeStrategy == ResizeStrategy::FIXED_ASPECT_RATIO_EXPAND_RES) {
                double scale = std::min((double)event.size.width / camera.aspectRatio.x, (double)event.size.height / camera.aspectRatio.y);

                math::Vector2d scaledRes = {scale * camera.aspectRatio.x, scale * camera.aspectRatio.y};

                if (scaledRes.x != event.size.width) {
                    camera.res.x = (unsigned)((double)event.size.width / (double)event.size.height * (double)camera.res.y);
                } else {
                    camera.res.y = (unsigned)((double)event.size.height / (double)event.size.width * (double)camera.res.x);
                }

                window->setView(sf::View({0, 0, (float)camera.res.x, (float)camera.res.y}));
            }
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                if (event.mouseWheelScroll.delta == 1) {
                    camera.zoom *= 0.9;
                } else if (event.mouseWheelScroll.delta == -1) {
                    camera.zoom *= 1.1;
                }
            }
        }

        // if (event.type == sf::Event::MouseMoved) {
        //     sf::View view = window->getView();
        //     view.setCenter(sf::Vector2f{sf::Mouse::getPosition()});
        //     window->setView(view);
        // }

        if (event.type == sf::Event::KeyPressed) {
            engineEvent.type = EventType::Key;
            constexpr float delta = 5.f;

            if (event.key.code == sf::Keyboard::J) {
                moveCamera({-delta, 0});
            } else if (event.key.code == sf::Keyboard::L) {
                moveCamera({delta, 0});
            } else if (event.key.code == sf::Keyboard::I) {
                moveCamera({0, -delta});
            } else if (event.key.code == sf::Keyboard::K) {
                moveCamera({0, delta});
            }
        }
    }
    for (auto & handler : eventHandlers) {
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
    if (gameLoop)
        gameLoop(dt);
}

void Engine::render()
{
    sf::View view = window->getView();
    if (camera.following != nullptr) {
        auto pos = camera.following->getPosition();
        view.setCenter(pos.x, pos.y);
    } else {
        view.setCenter(camera.pos.x, camera.pos.y);
    }
    view.zoom(camera.zoom);
    window->setView(view);

    window->clear(sf::Color::White);

    scene.draw(*window);

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

    window->display();
}

} // namespace iso
