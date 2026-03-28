#ifndef SPELL_AREA_DAMAGE_H
#define SPELL_AREA_DAMAGE_H

#include "spell_card.h"

class Spell_area_damage : public Spell_card{
private:
    int damage;
    int area_size;
    bool enhanced;

public:
    Spell_area_damage(int spell_damage = SPELL_AREA_DAMAGE_DAMAGE, int size = SPELL_AREA_SIZE, bool is_enhanced = false);
    bool can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const override;
    void cast(Player& player, int target_x, int target_y, Game_field& field) override;
    Spell_card* clone() const override;
    void enhance() override; 
    Target_type get_target_type() const override;
};

#endif