#include "Trap.h"

Trap::Trap(const std::string& n, Position p, int d, bool act)
    : name(n), pos(p), damage(d), active(act)
{}