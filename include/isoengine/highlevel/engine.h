#ifndef ENGINE_H
#define ENGINE_H

#include "isoengine/render/animator.h"
#include "isoengine/render/window.h"

namespace iso
{

class Engine
{
public:
    Engine();
    void run();

private:
    Window window;
    const float timePerFrame = 1.f / 60;
    Sprite bgSprite;
    std::vector<iso::Animator> animators;
    float zoom = 1;

    void handleInput();
    void update(float dt);
    void render();
};

} // namespace iso

#endif // ENGINE_H
