#ifndef GAMETYPES_H
#define GAMETYPES_H

enum class CellType {
    EMPTY,
    BLOCKED,
    SLOW,
    BUILDING
};

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class CombatMode {
    MELEE,
    RANGED
};

#endif