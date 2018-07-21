#ifndef TEXTURE_H
#define TEXTURE_H

#include "isoengine/math/vector2.h"
#include <SFML/Graphics/Texture.hpp>
#include <string>

namespace iso
{

class Texture
{
private:
    sf::Texture texture;

public:
    Texture() = default;

    bool loadFromFile(const std::string & filename);

    void setSmooth(bool smooth);

    iso::math::Vector2 getSize();

    const sf::Texture & getTexture() const
    {
        return texture;
    }
};

} // namespace iso

#endif // TEXTURE_H
