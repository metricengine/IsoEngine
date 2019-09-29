#include "isoengine/render/font.h"

namespace iso
{

bool Font::loadFromFile(const std::string & filename)
{
    return font.loadFromFile(filename);
}

const sf::Font & Font::getFont() const
{
    return font;
}

} // namespace iso