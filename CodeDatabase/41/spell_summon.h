#ifndef SPELL_SUMMON_H
#define SPELL_SUMMON_H

#include "spell_card.h"
#include "constants.h"

class Spell_summon : public Spell_card{
private:
    int summon_count;

public:
    Spell_summon(int count = 1);
    bool can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const override;
    void cast(Player& player, int target_x, int target_y, Game_field& field) override;
    Spell_card* clone() const override;
    void enhance() override;
    Target_type get_target_type() const override { return Target_type::NONE; }
};

#endif