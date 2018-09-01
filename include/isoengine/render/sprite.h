#ifndef SPRITE_H
#define SPRITE_H

#include "isoengine/math/rect.h"
#include "texture.h"
#include <SFML/Graphics/Sprite.hpp>

namespace iso
{

class Sprite
{
public:
    void setTexture(const iso::Texture & texture);
    void setTextureRect(const iso::math::Recti & rect);
    void setOrigin(const iso::math::Vector2f & vector);
    void setPosition(const iso::math::Vector2f & vector);
    void setScale(float x, float y);
    void setSize(const iso::math::Vector2f & size);
    math::Vector2f getPosition() const;
    const sf::Sprite & getSprite() const
    {
        return sprite;
    }

private:
    sf::Sprite sprite;
};

} // namespace iso

#endif
