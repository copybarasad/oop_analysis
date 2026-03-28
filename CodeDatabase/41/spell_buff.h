#ifndef SPELL_BUFF_H
#define SPELL_BUFF_H

#include "spell_card.h"
#include "constants.h"

class Spell_buff : public Spell_card{
private:
    static int buff_stacks;

public:
    Spell_buff();
    bool can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const override;
    void cast(Player& player, int target_x, int target_y, Game_field& field) override;
    Spell_card* clone() const override;
    void enhance() override;
    Target_type get_target_type() const override;
    static void apply_buff(Spell_card* spell);
    static void reset_buff();
    static bool has_buff() { return buff_stacks > 0; }
};

#endif