#include "isoengine/render/sprite.h"

namespace iso
{
void Sprite::setOrigin(const iso::math::Vector2 & vector)
{
    sprite.setOrigin(vector.x, vector.y);
}

void Sprite::setPosition(const iso::math::Vector2 & vector)
{
    sprite.setPosition(vector.x, vector.y);
}

void Sprite::setTexture(const iso::Texture & texture)
{
    sprite.setTexture(texture.getTexture());
}

void Sprite::setScale(float x, float y)
{
    sprite.setScale(x, y);
}
} // namespace iso
