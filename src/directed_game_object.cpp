#include "isoengine/directed_game_object.h"

namespace iso
{

DirectedGameObject::DirectedGameObject(
    const DirectedAnimations & animations,
    const math::Vector2f & dir)
{
    setDirectedAnimations(animations);
    move(dir, 0.f);
}

DirectedGameObject::DirectedGameObject(
    const Animations & animations,
    const math::Vector2f & dir)
{
    setDirectedAnimations(createDirectedAnimations(animations));
    move(dir, 0.f);
}

DirectedGameObject::DirectedAnimations DirectedGameObject::createDirectedAnimations(
    const std::vector<SharedAnimation> & animations)
{
    DirectedAnimations directedAnimations;
    float w = 360.f / float(animations.size());
    for (size_t i = 0; i < animations.size(); ++i) {
        float c = w * float(i);
        float lower = c - w / 2;
        float upper = c + w / 2;
        DirectionInterval interval{lower, upper};
        directedAnimations.push_back(
            DirectedAnimation{interval, animations[i]});
    }
    return directedAnimations;
}

void DirectedGameObject::setAnimations(const Animations & animations)
{
    setDirectedAnimations(createDirectedAnimations(animations));
}

void DirectedGameObject::move(const math::Vector2f & dir, float speed)
{
    facingDir = dir.normalize();
    float angle = std::atan2(-facingDir.y, facingDir.x) * 180.f / M_PI;

    if (angle < 0.f) {
        angle = 360.f + angle;
    }
    angle += 360.f;

    for (const auto & a : animations) {
        float l = a.interval.first + 360.f;
        float u = a.interval.second + 360.f;
        if (l <= angle && angle < u) {
            setAnimation(a.animation);
            break;
        }
    }
    GameObject::move(dir, speed);
}

void DirectedGameObject::setDirectedAnimations(
    const DirectedAnimations & animations)
{
    this->animations = animations;
}

const math::Vector2f & DirectedGameObject::facingDirection() const
{
    return facingDir;
}

} // namespace iso
