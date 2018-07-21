#include "isoengine/render/sprite.h"

namespace iso
{
void Sprite::setOrigin(float x, float y)
{
    sprite.setOrigin(x, y);
}

void Sprite::setPosition(const iso::math::Vector2 & vector)
{
    sprite.setPosition(vector.x, vector.y);
}

void Sprite::setTexture(const iso::Texture & texture)
{
    sprite.setTexture(texture.getTexture());
}
} // namespace iso
