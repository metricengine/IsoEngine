#ifndef ENGINE_H
#define ENGINE_H

#include "isoengine/events/commandqueue.h"
#include "isoengine/events/event.h"
#include "isoengine/highlevel/gameobject.h"
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
    Engine();
    void run();
    void addEventHandler(EventHandler eventHandler);
    void registerCommand(HashedString command);
    void addCommandHandler(CommandHandler cmdHandler);
    void addGameObject(std::shared_ptr<GameObject> gameObject);

private:
    Window window;
    const float timePerFrame = 1.f / 60;
    Sprite bgSprite;
    std::vector<Animator> animators;
    float zoom = 1;
    std::vector<EventHandler> eventHandlers;
    std::vector<CommandHandler> commandHandlers;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    CommandQueue commandQueue;

    void handleInput();
    void update(float dt);
    void render();
};

} // namespace iso

#endif // ENGINE_H
