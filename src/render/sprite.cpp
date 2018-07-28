#include "isoengine/render/sprite.h"

namespace iso
{
void Sprite::setOrigin(const iso::math::Vector2f & vector)
{
    sprite.setOrigin(vector.x, vector.y);
}

void Sprite::setPosition(const iso::math::Vector2f & vector)
{
    sprite.setPosition(vector.x, vector.y);
}

void Sprite::setTexture(const iso::Texture & texture)
{
    sprite.setTexture(texture.getTexture());
}

void Sprite::setTextureRect(const iso::math::Recti & rect)
{
    sprite.setTextureRect({rect.x, rect.y, rect.width, rect.height});
}

void Sprite::setScale(float x, float y)
{
    sprite.setScale(x, y);
}

} // namespace iso
