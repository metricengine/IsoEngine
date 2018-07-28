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
    Animation(const Texture & texture, const Vector2i & firstFrame, const Vector2i & frameSize, int numFrames, float duration, bool loop);
    void setFrame(Sprite & sprite, float time);

private:
    std::vector<Recti> frames;
    const Texture & texture;
    float duration;
    bool loop;
};

} // namespace iso

#endif // ANIMATION_H
