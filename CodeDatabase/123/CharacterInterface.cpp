#include "CharacterInterface.h"

Texture CharacterInterface::mCharacterTexture;
// Font CharacterInterface::mFont;
bool CharacterInterface::mIsDataLoad = false;

CharacterInterface::CharacterInterface(Character *ent)
{
    mCharacter = ent;
    if (!mIsDataLoad)
    {
        mCharacterTexture.loadFromFile("Textures/Characters.png");
        // mFont.openFromFile("arial.ttf");
        mIsDataLoad = true;
    }
}

// CharacterInterface::CharacterInterface(shared_ptr<Entity> ent)
// {
//     mCharacter = shared_ptr<Character>((Character *)ent.get());
//     mCharacterTexture.loadFromFile("Textures/Characters.png");
//     mFont.openFromFile("arial.ttf");
// }

void CharacterInterface::setTexture(Texture &texture)
{
    mCharacterTexture = texture;
}

void CharacterInterface::setPosition(Vector2f pos)
{
    mPos = pos;
}

Vector2f CharacterInterface::getPosition() const
{
    return mPos;
}

void CharacterInterface::setSize(Vector2f size)
{
    mSize = size;
}

Vector2f CharacterInterface::getSize() const
{
    return mSize;
}

void CharacterInterface::render(RenderWindow &window)
{
    int i = (int)mCharacter->getType() - 1;
    Sprite ent(mCharacterTexture);
    // ent.setTexture(&mCharacterTexture);
    IntRect a({i * 48, 0}, {48, 48});
    ent.setTextureRect(a);
    ent.setPosition({mPos.x + 3, mPos.y + 3});

    Text txt(mFont, std::to_string(mCharacter->getLife()) + "/" + std::to_string(mCharacter->getMaxLife()));
    txt.setPosition({mPos.x + mSize.x - txt.getCharacterSize() * 3, mPos.y});
    window.draw(txt);

    if (mCharacter->getType() != CharacterType::Building)
    {
        txt.setPosition({ent.getPosition().x, 48 + 5});
        String text = "MeleeCombatDamage: " + std::to_string(mCharacter->getMeleeDamage()) + "\n" +
                      "LongRangeCombatDamage: " + std::to_string(mCharacter->getLongRangeCombatDamage()) + "\n";
        if (mCharacter->isSlowed())
            text += "Is slowed: True\n";
        else
            text += "Is slowed: False\n";

        if (mCharacter->getType() == CharacterType::Player || mCharacter->getType() == CharacterType::Enemy)
        {
            text += "Hit type: ";
            switch (mCharacter->getHitType())
            {
            case HitType::LongRangeCombat:
                text += "Long-Range combat\n";
                break;

            case HitType::CloseCombat:
                text += "Close combat\n";
                break;

            case HitType::Hand:
                text += "Hand\n";
                break;

            default:
                break;
            }

            text += "Experience: " + std::to_string(mCharacter->getExp()) + "\n";
        }
        txt.setString(text);
        txt.setCharacterSize(20);
        window.draw(txt);
    }
    window.draw(ent);
}
