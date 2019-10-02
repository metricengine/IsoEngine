#include "isoengine/highlevel/engine.h"
#include "isoengine/common/clock.h"
#include "isoengine/events/sfml_events.h"
#include <SFML/Graphics.hpp>
#include <exception>

namespace iso
{

Engine::Engine(const WindowOptions & windowOpts, std::initializer_list<HashedString> layerNames)
    : scene{layerNames}
{
    switch (windowOpts.resizeStrategy) {
    case ResizeStrategy::FIXED_RES_STATIC_WINDOW:
        window = std::make_unique<render::Window>("IsoEngine", windowOpts.resolution, WindowStyle::Static);
        break;
    case ResizeStrategy::FIXED_RES_STRETCH:
    case ResizeStrategy::DYNAMIC_RES:
    // case ResizeStrategy::FIXED_ASPECT_RATIO:
    case ResizeStrategy::FIXED_ASPECT_RATIO_EXPAND_RES:
        window = std::make_unique<render::Window>("IsoEngine", windowOpts.resolution, WindowStyle::Resize);
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

    collisionDetector = std::make_unique<physics::CollisionDetector>(
        math::Rectf(0, 0, windowOpts.resolution.x, windowOpts.resolution.y));
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

void Engine::registerCommand(HashedString command)
{
    commandQueue.registerCommand(command);
}

void Engine::addGameObject(std::shared_ptr<GameObject> gameObject)
{
    addGameObject(gameObject, scene.topLayer());
}

void Engine::addGameObject(std::shared_ptr<GameObject> gameObject, HashedString layerName)
{
    addGameObject(gameObject, scene.getLayer(layerName));
}

void Engine::addGameObject(std::shared_ptr<GameObject> gameObject, render::SceneNode & layer)
{
    addSceneNode(gameObject, layer);
    gameObjects.push_back(gameObject);
    gameObject->setCommandQueue(&commandQueue);
}

void Engine::removeGameObject(const GameObject * gameObject)
{
    removeGameObject(gameObject, scene.topLayer());
}

void Engine::removeGameObject(const GameObject * gameObject, HashedString layerName)
{
    removeGameObject(gameObject, scene.getLayer(layerName));
}

void Engine::removeGameObject(
    const GameObject * gameObject,
    render::SceneNode & layer)
{
    for (auto iter = gameObjects.begin(); iter != gameObjects.end(); ++iter) {
        if (gameObject == iter->get()) {
            gameObjects.erase(iter);
            break;
        }
    }
    removeSceneNode(gameObject, layer);
    if (gameObject->collisionDetector != nullptr) {
        collisionDetector->removeRigidBody(gameObject);
    }
}

void Engine::addSceneNode(std::shared_ptr<render::SceneNode> sceneNode)
{
    addSceneNode(sceneNode, scene.topLayer());
}

void Engine::addSceneNode(std::shared_ptr<render::SceneNode> sceneNode, HashedString layer)
{
    addSceneNode(sceneNode, scene.getLayer(layer));
}

void Engine::addSceneNode(std::shared_ptr<render::SceneNode> sceneNode, render::SceneNode & layer)
{
    layer.addChild(sceneNode);
}

void Engine::removeSceneNode(const render::SceneNode * sceneNode)
{
    removeSceneNode(sceneNode, scene.topLayer());
}

void Engine::removeSceneNode(const render::SceneNode * sceneNode, HashedString layer)
{
    removeSceneNode(sceneNode, scene.getLayer(layer));
}

void Engine::removeSceneNode(const render::SceneNode * sceneNode, render::SceneNode & layer)
{
    layer.removeChild(sceneNode);
}

void Engine::moveCamera(math::Vector2f dir)
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

void Engine::addRigidBody(std::shared_ptr<GameObject> gameObject)
{
    addRigidBody(
        gameObject,
        math::Rectf(math::Vector2f(), gameObject->getSprite().getSize()));
}

void Engine::addRigidBody(std::shared_ptr<GameObject> gameObject, const math::Rectf & boundingBox)
{
    gameObject->setCollisionDetector(collisionDetector.get());
    gameObject->boundingBox = boundingBox;
    collisionDetector->addRigidBody(gameObject);
}

void Engine::registerGameObject(std::shared_ptr<GameObject> gameObject)
{
    gameObjects.push_back(gameObject);
    gameObject->setCommandQueue(&commandQueue);
}

void Engine::handleInput()
{
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }

        if (event.type == sf::Event::MouseButtonPressed ||
            event.type == sf::Event::MouseButtonReleased ||
            event.type == sf::Event::MouseMoved) {
            auto mouseEvent = events::mouseEventFromSFML(event);
            onMouse.raise(mouseEvent);
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

        // if (event.type == sf::Event::KeyPressed) {
        //     constexpr float delta = 5.f;

        //     if (event.key.code == sf::Keyboard::J) {
        //         moveCamera({-delta, 0});
        //     } else if (event.key.code == sf::Keyboard::L) {
        //         moveCamera({delta, 0});
        //     } else if (event.key.code == sf::Keyboard::I) {
        //         moveCamera({0, -delta});
        //     } else if (event.key.code == sf::Keyboard::K) {
        //         moveCamera({0, delta});
        //     }
        // }

        if (event.type == sf::Event::KeyPressed ||
            event.type == sf::Event::KeyReleased) {
            auto keyEvent = events::keyEventFromSFML(event);
            onKey.raise(keyEvent);
        }
    }
}

void Engine::update(float dt)
{
    while (!commandQueue.isEmpty()) {
        auto cmd = commandQueue.popCommand();
        auto & sender = *cmd.first;
        auto & cmdType = *cmd.second.get();

        onCommand.raise(sender, cmdType);

        for (auto gameObject : gameObjects)
            if (commandQueue.objectListensToCommand(*gameObject, cmdType))
                gameObject->handleCommand(sender, cmdType);
    }
    for (auto gameObject : gameObjects)
        gameObject->update(dt);
    onUpdate.raise(dt);
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
