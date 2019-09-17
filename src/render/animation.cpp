#include "isoengine/render/animation.h"

namespace iso
{

Animation::Animation(const Texture & texture, const Vector2i & frameSize, const Vector2i & firstFrame, int numFrames, float duration, bool loop) : texture(texture), duration(duration), loop(loop)
{
    frames.resize(numFrames);
    for (int i = 0; i < numFrames; ++i)
        frames[i] = Recti({firstFrame.x + frameSize.x * i, firstFrame.y}, frameSize);
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
