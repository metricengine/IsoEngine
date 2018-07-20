#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics/Texture.hpp>

namespace iso
{

class Texture : public sf::Texture
{
public:
    Texture() = default;
    bool loadFromFile(const std::string & filename);
    void setSmooth(bool smooth);
};

} // namespace iso

#endif // TEXTURE_H