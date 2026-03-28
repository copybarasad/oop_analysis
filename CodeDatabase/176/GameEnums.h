#ifndef GAME_ENUMS_H
#define GAME_ENUMS_H

enum class CombatMode {
    MELEE,
    RANGED
};

enum class CellType {
    EMPTY,
    OBSTACLE,
    SLOW
};

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

#endif

