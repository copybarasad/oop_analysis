#ifndef INCLUDE_HAND
#define INCLUDE_HAND

#include "../items/spell.h"

class Hand {
private:
    Spell* active_spell_;

public:
    Hand();
    ~Hand();
    
    bool has_spell() const { return active_spell_ != nullptr; }
    const Spell* active_spell() const { return active_spell_; }
    Spell* active_spell_copy();

    void equip_spell(Spell* spell);
    void clear();
    
    bool use_spell(class GameBoard& board);
};

#endif