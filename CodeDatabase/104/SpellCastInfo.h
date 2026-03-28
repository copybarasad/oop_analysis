#ifndef SPELLCASTINFO_H
#define SPELLCASTINFO_H

#include "Position.h"

class SpellCastInfo {
public:
    Position target_position; 
    bool requires_target;   

    explicit SpellCastInfo(const Position& target, bool requires_target = false)
        : target_position(target), requires_target(requires_target) {
    }
};

#endif