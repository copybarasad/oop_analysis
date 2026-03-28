#ifndef SPELL_TRAP_H
#define SPELL_TRAP_H

#include "spell_card.h"
#include "constants.h"

class Spell_trap : public Spell_card{
private:
    int damage;

public:
    Spell_trap(int damage = SPELL_TRAP_DAMAGE);
    bool can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const override;
    void cast(Player& player, int target_x, int target_y, Game_field& field) override;
    Spell_card* clone() const override;
    void enhance() override;
    Target_type get_target_type() const override;
};

#endif