#ifndef DAMAGE_SPELL_HPP
#define DAMAGE_SPELL_HPP

#include "spell_card.hpp"
#include <string>

class Entity;

class DirectDamageSpell : public SpellCard {
private:
    std::string name = "Direct Damage";
    int range;
    int damage;

public:
    DirectDamageSpell(int range_, int dmg);

    std::string getName() const override;
    int getRange() const override;

    int getDamage() const;
};

#endif
