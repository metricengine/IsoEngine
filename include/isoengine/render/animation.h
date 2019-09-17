#ifndef ANIMATION_H
#define ANIMATION_H

#include "isoengine/math/rect.h"
#include "isoengine/math/vector2.h"
#include "isoengine/render/sprite.h"
#include "isoengine/render/texture.h"
#include <vector>

namespace iso
{
using namespace math;

class Animation
{
public:
    Animation(const Texture & texture, const Vector2i & frameSize, const Vector2i & firstFrame, int numFrames, float duration, bool loop);
    virtual void setFrame(Sprite & sprite, float time);

protected:
    std::vector<Recti> frames;
    const Texture & texture;
    float duration;
    bool loop;
};

class StaticAnimation : public Animation
{
public:
    StaticAnimation(const Texture & texture, const Vector2i frameSize, const Vector2i frameOffset = {0, 0}) : Animation(texture, frameSize, frameOffset, 1, 0, false){};
    void setFrame(Sprite & sprite, float time) override;
};

} // namespace iso

#endif // ANIMATION_H
