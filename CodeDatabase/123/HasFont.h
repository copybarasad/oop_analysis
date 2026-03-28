#ifndef HAS_FONT
#define HAS_FONT

#include <SFML/Graphics.hpp>

class HasFont
{
protected:
    static sf::Font mFont;
    static bool mIsFontLoaded;

public:
    HasFont();
};

#endif