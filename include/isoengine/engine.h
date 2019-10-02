#ifndef ENGINE_H
#define ENGINE_H

#include "isoengine/events/command_queue.h"
#include "isoengine/events/delegates.h"
#include "isoengine/events/event.h"
#include "isoengine/game_object.h"
#include "isoengine/render/render_scene.h"
#include "isoengine/physics/collision_detector.h"
#include "isoengine/render/window.h"

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
    events::Delegates<void(GameObject &, const events::Command &)> onCommand;
    events::Delegates<void(events::KeyEvent)> onKey;
    events::Delegates<void(events::MouseEvent)> onMouse;
    events::Delegates<void(float)> onUpdate;

    // Methods
    Engine(const WindowOptions & windowOpts, std::initializer_list<support::HashedString> layerNames = {});
    void run();
    void registerCommand(support::HashedString command);
    void addGameObject(
        std::shared_ptr<GameObject> gameObject);
    void addGameObject(
        std::shared_ptr<GameObject> gameObject,
        support::HashedString layer);
    void removeGameObject(const GameObject * gameObject);
    void removeGameObject(const GameObject * gameObject, support::HashedString layer);
    void addSceneNode(std::shared_ptr<render::SceneNode> sceneNode);
    void addSceneNode(std::shared_ptr<render::SceneNode> sceneNode, support::HashedString layer);
    void removeSceneNode(const render::SceneNode * sceneNode);
    void removeSceneNode(const render::SceneNode * sceneNode, support::HashedString layer);
    void addRigidBody(std::shared_ptr<GameObject> gameObject);
    void addRigidBody(std::shared_ptr<GameObject> gameObject, const math::Rectf & boundingBox);
    void registerGameObject(std::shared_ptr<GameObject> gameObject);
    void moveCamera(math::Vector2f dir);
    void zoomCamera(float scale);
    void cameraFollowObject(const GameObject * obj);
    void cameraStopFollowing();
    math::Vector2f screenToWorldCoords(const math::Vector2i & coords);

private:
    struct Camera {
        float zoom = 1;
        math::Vector2f pos;
        math::Vector2u res;
        math::Vector2u aspectRatio;
        const GameObject * following = nullptr;
    };

    void addGameObject(
        std::shared_ptr<GameObject> gameObject,
        render::SceneNode & layer);
    void removeGameObject(
        const GameObject * gameObject,
        render::SceneNode & layer);
    void addSceneNode(
        std::shared_ptr<render::SceneNode> sceneNode,
        render::SceneNode & layer);
    void removeSceneNode(
        const render::SceneNode * sceneNode,
        render::SceneNode & layer);

    // Model
    const float timePerFrame = 1.f / 60.f;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    events::CommandQueue commandQueue;
    // Rendering
    ResizeStrategy resizeStrategy;
    Camera camera;
    std::unique_ptr<render::Window> window;
    std::unique_ptr<physics::CollisionDetector> collisionDetector;
    render::RenderScene scene;

    void handleInput();
    void update(float dt);
    void render();
};

} // namespace iso

#endif // ENGINE_H
