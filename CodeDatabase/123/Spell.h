#ifndef SPELL
#define SPELL

// #include "SFML/Graphics.hpp"
#include "../Map.h"

#define SPELL_CAPASITY 2

class Spell
{
protected:
    string mName;
    unsigned short mLevel;

public:
    virtual void use(Map &map, shared_ptr<Character> Speller, Vec2 pos) = 0;

    string getName() const
    {
        return mName;
    }

    unsigned short getLevel() const
    {
        return mLevel;
    }

    virtual Spell *clone() const = 0;

    void update()
    {
        mLevel++;
        setLevel();
    }

protected:
    virtual void setLevel() = 0;

    // bool operator==(const Spell &p)
    // {
    //     return mSpellType == p.mSpellType;
    // }

    // bool operator!=(const Spell &p)
    // {
    //     return mSpellType != p.mSpellType;
    // }
};

#endif