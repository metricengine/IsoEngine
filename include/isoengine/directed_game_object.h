#ifndef DIRECTED_GAME_OBJECT_H
#define DIRECTED_GAME_OBJECT_H

#include "game_object.h"
#include "isoengine/render/animation.h"

namespace iso
{

class DirectedGameObject : public GameObject
{
    // Interval of angles (lower_bound, upper_bound)
    using DirectionInterval = std::pair<float, float>;
    using SharedAnimation = std::shared_ptr<render::Animation>;

    struct DirectedAnimation {
        DirectionInterval interval;
        SharedAnimation animation;
    };
    using Animations = std::vector<SharedAnimation>;
    using DirectedAnimations = std::vector<DirectedAnimation>;

public:
    DirectedGameObject() {}
    DirectedGameObject(
        const Animations & animations,
        const math::Vector2f & dir);
    DirectedGameObject(
        const DirectedAnimations & animations,
        const math::Vector2f & dir);
    void move(const math::Vector2f & dir, float speed = 1.f) override;
    void setDirectedAnimations(const DirectedAnimations & animations);
    void setAnimations(const Animations & animations);
    const math::Vector2f & facingDirection() const;

    // Animations have to specified from facing right in a counterclockwise directions
    static DirectedAnimations createDirectedAnimations(
        const std::vector<SharedAnimation> & animations);

private:
    math::Vector2f facingDir;
    DirectedAnimations animations;
};

} // namespace iso

#endif // DIRECTED_GAME_OBJECT_H
