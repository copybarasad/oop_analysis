#ifndef FIREBALL_SPELL_H
#define FIREBALL_SPELL_H

#include "spell.h"

class Entity;

class FireballSpell : public Spell {
private:
    int area_size;
    
public:
    FireballSpell();
    
    bool canCast(game_field* field, int player_x, int player_y, int target_x, int target_y) const override;
    
    void apply(game_field* field, int player_x, int player_y, int target_x, int target_y, Entity* target = nullptr) override;
    
    int getAreaSize() const;
};

#endif