#include "HandInterface.h"

// Font HandInterface::mFont;
// bool HandInterface::mIsDataLoad = false;

void HandInterface::setHand(Hand *h)
{
    mHand = h;
}

void HandInterface::setPosition(Vector2f pos)
{
    mPos = pos;
}

Vector2f HandInterface::getPosition() const
{
    return mPos;
}

void HandInterface::render(RenderWindow &window)
{
    Text txt(mFont);
    txt.setCharacterSize(20);

    string buff = "";
    for (size_t i = 0; i < mHand->getSize(); i++)
    {
        // buff += to_string(i + 1) + ". " + mHand->getSpell(i)->getName();
        buff += format("{}. {} (lvl {})", i + 1, mHand->getSpell(i)->getName(), mHand->getSpell(i)->getLevel() + 1);
        if (i == mHand->getSelection())
            buff += " <--";
        buff += "\n";
    }

    txt.setString(buff);
    txt.setPosition(mPos);
    window.draw(txt);
}
