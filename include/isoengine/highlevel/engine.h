#ifndef ENGINE_H
#define ENGINE_H

#include "isoengine/events/commandqueue.h"
#include "isoengine/events/delegates.h"
#include "isoengine/events/event.h"
#include "isoengine/highlevel/gameobject.h"
#include "isoengine/highlevel/renderscene.h"
#include "isoengine/render/window.h"
#include <functional>

namespace iso
{

enum class ResizeStrategy {
    // Resolution is fixed and window cannot be resized, this is the default mode
    FIXED_RES_STATIC_WINDOW,
    // Resolution is fixed and image is stretched to fit window
    FIXED_RES_STRETCH,
    // Change resolution based on window size
    DYNAMIC_RES,
    // Stretch only until aspect ratio is preserved, don't allow resizing beyond that (for some reason that doesn't work)
    // FIXED_ASPECT_RATIO,
    // Stretch image until aspect ratio is preserved and expand resolution to cover the rest of window
    FIXED_ASPECT_RATIO_EXPAND_RES
};

struct WindowOptions {
    WindowOptions(const math::Vector2u & resolution, ResizeStrategy resizeStrategy = ResizeStrategy::FIXED_RES_STATIC_WINDOW, const math::Vector2u & aspectRatio = {}) : resizeStrategy(resizeStrategy), resolution(resolution), aspectRatio(aspectRatio) {}

    ResizeStrategy resizeStrategy;
    math::Vector2u resolution;
    math::Vector2u aspectRatio;
};

class Engine
{
public:
    // Events
    Delegates<void(GameObject &, const Command &)> onCommand;
    Delegates<void(KeyEvent)> onKey;
    Delegates<void(MouseEvent)> onMouse;
    Delegates<void(float)> onUpdate;

    // Methods
    Engine(const WindowOptions & windowOpts, std::initializer_list<HashedString> layerNames = {});
    void run();
    void registerCommand(HashedString command);
    void addGameObject(std::shared_ptr<GameObject> gameObject);
    void addGameObject(std::shared_ptr<GameObject> gameObject, HashedString layer);
    void registerGameObject(std::shared_ptr<GameObject> gameObject);
    void moveCamera(Vector2f dir);
    void zoomCamera(float scale);
    void cameraFollowObject(const GameObject * obj);
    void cameraStopFollowing();
    math::Vector2f screenToWorldCoords(const math::Vector2i & coords);

private:
    struct Camera {
        float zoom = 1;
        Vector2f pos;
        Vector2u res;
        Vector2u aspectRatio;
        const GameObject * following = nullptr;
    };

    // Model
    const float timePerFrame = 1.f / 60;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    CommandQueue commandQueue;
    // Rendering
    ResizeStrategy resizeStrategy;
    Camera camera;
    std::unique_ptr<Window> window;
    RenderScene scene;

    void handleInput();
    void update(float dt);
    void render();
};

} // namespace iso

#endif // ENGINE_H
