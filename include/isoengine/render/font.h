#ifndef FONT_H
#define FONT_H

#include <SFML/Graphics/Font.hpp>
#include <string>

namespace iso
{

class Font
{
public:
    bool loadFromFile(const std::string & filename);
    const sf::Font & getFont() const;

private:
    sf::Font font;
};

} // namespace iso

#endif // FONT_H
