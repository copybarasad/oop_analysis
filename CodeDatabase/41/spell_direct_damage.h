#ifndef SPELL_DIRECT_DAMAGE_H
#define SPELL_DIRECT_DAMAGE_H

#include "spell_card.h"

class Spell_direct_damage : public Spell_card{
private:
    int damage;
    bool enhanced;

public:
    Spell_direct_damage(int spell_damage = SPELL_DIRECT_DAMAGE_DAMAGE, bool is_enhanced = false);
    bool can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const override;
    void cast(Player& player, int target_x, int target_y, Game_field& field) override;
    Spell_card* clone() const override;
    void enhance() override;
    Target_type get_target_type() const override;
};

#endif