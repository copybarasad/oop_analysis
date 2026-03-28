#include "HasFont.h"

sf::Font HasFont::mFont;
bool HasFont::mIsFontLoaded = false;

HasFont::HasFont()
{
    if (!mIsFontLoaded)
    {
        mFont.openFromFile("arial.ttf");
        mIsFontLoaded = true;
    }
}