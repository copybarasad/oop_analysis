#ifndef DIRECT_DAMAGE_SPELL
#define DIRECT_DAMAGE_SPELL

#include "Spell.h"

class DirectDamageSpell : public Spell
{
protected:
    int mDamage;

public:
    DirectDamageSpell(unsigned short lvl = 0);

    DirectDamageSpell(const DirectDamageSpell &other);

    void use(Map &map, shared_ptr<Character> Speller, sf::Vector2i pos) override;

    Spell *clone() const override;

protected:
    void setLevel() override;
};

#endif