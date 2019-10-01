#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "animation.h"
#include <memory>

namespace iso::render
{

class Animator
{
public:
    void update(float duration);
    void setAnimation(std::shared_ptr<Animation> animation);

    const Sprite & getSprite() const
    {
        return sprite;
    }
    Sprite & getSprite()
    {
        return sprite;
    }
    void setAnimationSpeed(float s)
    {
        speed = s;
    }

private:
    Sprite sprite;
    float time;
    float speed = 1.f;
    std::shared_ptr<Animation> animation;
};

} // namespace iso

#endif // ANIMATOR_H
