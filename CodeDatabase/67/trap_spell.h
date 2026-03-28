#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "spell.h"
class Entity;

class TrapSpell : public Spell {
public:
    TrapSpell();
    
    bool canCast(game_field* field, int player_x, int player_y, int target_x, int target_y) const override;
    void apply(game_field* field, int player_x, int player_y, int target_x, int target_y, Entity* target = nullptr) override;
};
#endif
