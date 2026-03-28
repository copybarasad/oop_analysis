#ifndef SPELLTARGET_H
#define SPELLTARGET_H

#include "Position.h"

struct SpellTarget {
    Position targetPosition;
    
    SpellTarget(const Position& pos = Position(0, 0))
        : targetPosition(pos) {}
};

#endif
