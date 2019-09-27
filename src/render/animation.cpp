#include "isoengine/render/animation.h"

namespace iso
{

Animation::Animation(
    const Texture & texture,
    const Vector2i & frameSize,
    const Vector2i & firstFrame,
    const Vector2i & numFrames,
    float duration,
    bool loop,
    const Vector2i & frameOffset)
    : texture(texture), duration(duration), loop(loop)
{
    frames.resize(numFrames.x * numFrames.y);
    int idx = 0;
    for (int i = 0; i < numFrames.y; ++i) {
        for (int j = 0; j < numFrames.x; ++j) {
            frames[idx] = Recti(
                firstFrame.x + (frameSize.x + frameOffset.x) * j,
                firstFrame.y + (frameSize.y + frameOffset.y) * i,
                frameSize.x,
                frameSize.y);
            ++idx;
        }
    }
}

void Animation::setFrame(Sprite & sprite, float time)
{
    // normalized time
    float t{time / duration};
    size_t currentFrame{static_cast<size_t>(t * frames.size())};

    if (loop)
        currentFrame %= frames.size();
    else if (currentFrame >= frames.size())
        currentFrame = frames.size() - 1;

    sprite.setTexture(texture);
    sprite.setTextureRect(frames[currentFrame]);
}

void StaticAnimation::setFrame(Sprite & sprite, float time)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(frames[0]);
}

} // namespace iso
