#ifndef TEXT_H
#define TEXT_H

#include "font.h"
#include "isoengine/math/vector2.h"
#include <SFML/Graphics/Text.hpp>
#include <string>

namespace iso::render
{

class Font;

class Text
{
public:
    Text(const std::string & string = "");
    void setFont(const Font & font);
    void setString(const std::string & string);
    std::string getString() const;
    const sf::Text & getText() const;
    math::Vector2f getSize() const;

private:
    sf::Text text;
};

} // namespace iso

#endif // TEXT_H
