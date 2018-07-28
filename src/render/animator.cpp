#include "isoengine/render/animator.h"

namespace iso
{

void Animator::update(float duration)
{
    time += duration * speed;
    animation->setFrame(sprite, time);
}

} // namespace iso