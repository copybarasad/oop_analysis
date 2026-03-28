#include "SquareDamageSpell.h"

SquareDamageSpell::SquareDamageSpell(unsigned short lvl)
{
    // mSpellType = SpellType::SquareDamage;
    mDamage = 5;
    mSize = 2;
    mLevel = lvl;
    setLevel();
    mName = "Square damage";
}

SquareDamageSpell::SquareDamageSpell(const SquareDamageSpell &other)
{
    // mDamage = other.mDamage;
    mSize = other.mSize;
}

void SquareDamageSpell::use(Map &map, shared_ptr<Character> Speller, sf::Vector2i pos)
{
    for (int y = 0; y < mSize; y++)
    {
        for (int x = 0; x < mSize; x++)
        {
            Vector2i buff = pos + Vector2i(x, y);
            if (buff.x >= 0 && buff.y >= 0 && buff.x < map.getSize().x && buff.y < map.getSize().y)
            {
                if (map[buff.y][buff.x].ent != nullptr)
                {
                    map[buff.y][buff.x].ent->dealDamage(mDamage);
                    if (map[buff.y][buff.x].ent->isDead())
                        Speller->addExp(map[buff.y][buff.x].ent->getExpForKill());
                }
            }
        }
    }
}

Spell *SquareDamageSpell::clone() const
{
    return new SquareDamageSpell(*this);
}

void SquareDamageSpell::setLevel()
{
    if (mLevel == 0)
    {
        mDamage = 5;
    }
    else if (mLevel == 1)
    {
        mDamage = 7;
    }
    else if (mLevel == 2)
    {
        mDamage = 10;
    }
    else if (mLevel == 3)
    {
        mDamage = 13;
    }
    else
    {
        mDamage = 15;
        mSize = 3;
    }
}
