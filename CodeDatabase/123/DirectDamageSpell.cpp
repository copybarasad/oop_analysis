#include "DirectDamageSpell.h"

DirectDamageSpell::DirectDamageSpell(unsigned short lvl)
{
    // mSpellType = SpellType::DirectDamage;
    // mDamage = 10;
    mLevel = lvl;
    setLevel();
    mName = "Direct damage";
}

DirectDamageSpell::DirectDamageSpell(const DirectDamageSpell &other)
{
    // mSpellType = other.mSpellType;
    mDamage = other.mDamage;
}

void DirectDamageSpell::use(Map &map, shared_ptr<Character> Speller, sf::Vector2i pos)
{
    if (map[pos.y][pos.x].ent != nullptr)
    {
        map[pos.y][pos.x].ent->dealDamage(mDamage);

        if (map[pos.y][pos.x].ent->isDead())
            Speller->addExp(map[pos.y][pos.x].ent->getExpForKill());
    }
}

Spell *DirectDamageSpell::clone() const
{
    return new DirectDamageSpell(*this);
}

void DirectDamageSpell::setLevel()
{
    if (mLevel == 0)
    {
        mDamage = 10;
    }
    else if (mLevel == 1)
    {
        mDamage = 12;
    }
    else if (mLevel == 2)
    {
        mDamage = 15;
    }
    else if (mLevel == 3)
    {
        mDamage = 18;
    }
    else
    {
        mDamage = 20;
    }
}
