#include "isoengine/render/text.h"
#include <SFML/Graphics/Font.hpp>

namespace iso::render
{

Text::Text(const std::string & string)
{
    setString(string);
}

void Text::setFont(const Font & font)
{
    text.setFont(font.getFont());
}

void Text::setString(const std::string & string)
{
    text.setString(string);
}

std::string Text::getString() const
{
    return text.getString();
}

const sf::Text & Text::getText() const
{
    return text;
}

math::Vector2f Text::getSize() const
{
    return {text.getGlobalBounds().width,
            text.getGlobalBounds().height};
}

} // namespace iso
