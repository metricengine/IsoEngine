#include "isoengine/render/texture.h"

namespace iso
{

bool Texture::loadFromFile(const std::string & filename)
{
    return texture.loadFromFile(filename);
}

void Texture::setSmooth(bool smooth)
{
    texture.setSmooth(smooth);
}

iso::math::Vector2 Texture::getSize()
{
    return {texture.getSize().x, texture.getSize().y};
}

} // namespace iso
