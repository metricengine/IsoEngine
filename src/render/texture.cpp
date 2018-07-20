#include "isoengine/render/texture.h"

#include <iostream>

namespace iso
{

bool Texture::loadFromFile(const std::string & filename)
{
    return sf::Texture::loadFromFile(filename);
}

void Texture::setSmooth(bool smooth)
{
    sf::Texture::setSmooth(smooth);
}

} // namespace iso