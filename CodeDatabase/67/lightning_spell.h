#ifndef LIGHTNING_SPELL_H
#define LIGHTNING_SPELL_H

#include "spell.h"
class Entity;

class LightningSpell : public Spell {
public:
    LightningSpell();
    LightningSpell(int spell_damage, int spell_radius);
    bool canCast(game_field* field, int player_x, int player_y, int target_x, int target_y) const override;
    void apply(game_field* field, int player_x, int player_y, int target_x, int target_y, Entity* target = nullptr) override;
};

#endif
