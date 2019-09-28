#include "isoengine/render/animator.h"

namespace iso
{

void Animator::setAnimation(std::shared_ptr<Animation> animation)
{
    time = 0.f;
    this->animation = animation;
    update(time);
}

void Animator::update(float duration)
{
    time += duration * speed;
    animation->setFrame(sprite, time);
}

} // namespace iso