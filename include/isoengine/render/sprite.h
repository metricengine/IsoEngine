#ifndef SPRITE_H
#define SPRITE_H

#include "isoengine/math/rect.h"
#include "texture.h"
#include <SFML/Graphics/Sprite.hpp>

namespace iso::render
{

class Sprite
{
public:
    void setTexture(const Texture & texture);
    void setTextureRect(const iso::math::Recti & rect);
    void setOrigin(const iso::math::Vector2f & vector);
    void setPosition(const iso::math::Vector2f & vector);
    void setScale(float x, float y);
    math::Vector2f getSize() const;
    math::Vector2f getPosition() const;
    math::Vector2f getScale() const;
    const sf::Sprite & getSprite() const
    {
        return sprite;
    }

private:
    sf::Sprite sprite;
};

} // namespace iso

#endif
