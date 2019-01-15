#ifndef ENGINE_H
#define ENGINE_H

#include "isoengine/events/commandqueue.h"
#include "isoengine/events/event.h"
#include "isoengine/highlevel/gameobject.h"
#include "isoengine/highlevel/renderscene.h"
#include "isoengine/render/animator.h"
#include "isoengine/render/window.h"
#include <functional>

namespace iso
{
using EventHandler = std::function<void(const iso::Event &)>;
using CommandHandler = std::function<void(GameObject &, const Command &)>;

class Engine
{
public:
    Engine(std::initializer_list<HashedString> layerNames);
    void run();
    void addEventHandler(EventHandler eventHandler);
    void registerCommand(HashedString command);
    void addCommandHandler(CommandHandler cmdHandler);
    void addGameObject(std::shared_ptr<GameObject> gameObject);
    void addGameObject(std::shared_ptr<GameObject> gameObject, HashedString layer);

private:
    // I/O
    std::vector<EventHandler> eventHandlers;
    std::vector<CommandHandler> commandHandlers;
    // Model
    const float timePerFrame = 1.f / 60;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    CommandQueue commandQueue;
    // Rendering
    Window window;
    RenderScene scene;
    float zoom = 1;

    void handleInput();
    void update(float dt);
    void render();
};

} // namespace iso

#endif // ENGINE_H
