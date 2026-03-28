#ifndef AOE_SPELL_HPP
#define AOE_SPELL_HPP

#include "spell_card.hpp"
#include <string>

class AoeDamageSpell : public SpellCard {
private:
    std::string name = "AOE Damage";
    int range;
    int damage;

public:
    AoeDamageSpell(int range_, int dmg);

    std::string getName() const override;
    int getRange() const override;

    int getDamage() const;
};

#endif
