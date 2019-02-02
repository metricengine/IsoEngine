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
    void registerGameObject(std::shared_ptr<GameObject> gameObject);
    void moveCamera(Vector2f dir);
    void zoomCamera(float scale);
    void cameraFollowObject(const GameObject * obj);
    void cameraStopFollowing();

private:
    struct Camera {
        float zoom = 1;
        Vector2f pos;
        const GameObject * following = nullptr;
    };

    // I/O
    std::vector<EventHandler> eventHandlers;
    std::vector<CommandHandler> commandHandlers;
    // Model
    const float timePerFrame = 1.f / 60;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    CommandQueue commandQueue;
    // Rendering
    Camera camera;
    Window window;
    RenderScene scene;

    void handleInput();
    void update(float dt);
    void render();
};

} // namespace iso

#endif // ENGINE_H
