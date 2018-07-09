#include "isoengine/common/debug/profile.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

void test_profile()
{
    PROFILE_FUNCTION;

    double s = 0;
    for (int i = 0; i < 50000000; ++i)
        s += std::sin(i);

    {
        PROFILE_BLOCK("TestBlock");
        double s2 = 0;
        for (int i = 0; i < 1000000; ++i)
            s2 += std::cos(i);
        std::cout << s2 << std::endl;
    }

    std::cout << s << std::endl;
}

int main()
{
    test_profile();

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    ProfileManager::getInstance().showResult();

    return 0;
}