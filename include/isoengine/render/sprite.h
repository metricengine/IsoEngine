#ifndef SPRITE_H
#define SPRITE_H

#include "isoengine/math/vector2.h"
#include "texture.h"
#include <SFML/Graphics/Sprite.hpp>

namespace iso
{

class Sprite
{
private:
    sf::Sprite sprite;

public:
    void setTexture(const iso::Texture & texture);
    void setOrigin(float x, float y);
    void setPosition(const iso::math::Vector2 & vector);
    const sf::Sprite & getSprite() const
    {
        return sprite;
    }
};

} // namespace iso

#endif
