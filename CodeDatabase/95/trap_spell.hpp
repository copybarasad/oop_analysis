#ifndef TRAP_SPELL_HPP
#define TRAP_SPELL_HPP

#include "spell_card.hpp"
#include <string>

class TrapSpell : public SpellCard {
private:
    std::string name = "Trap Spell";
    int range;
    int damage;

public:
    TrapSpell(int range_, int dmg);

    std::string getName() const override;
    int getRange() const override;

    int getDamage() const;
};

#endif
