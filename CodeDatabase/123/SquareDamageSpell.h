#ifndef SQUARE_DAMAGE_SPELL
#define SQUARE_DAMAGE_SPELL

#include "Spell.h"

class SquareDamageSpell : public Spell
{
protected:
    int mDamage;
    int mSize = 2;

public:
    SquareDamageSpell(unsigned short lvl = 0);

    SquareDamageSpell(const SquareDamageSpell &other);

    void use(Map &map, shared_ptr<Character> Speller, sf::Vector2i pos) override;

    Spell *clone() const override;

protected:
    void setLevel() override;
};

#endif