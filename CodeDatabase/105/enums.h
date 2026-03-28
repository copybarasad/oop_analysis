#ifndef ENUMS_H
#define ENUMS_H

// Enum для типа боя
enum class CombatType {
    MELEE,
    RANGED
};

// Enum для типа клетки
enum class CellType {
    EMPTY,
    BLOCKED,
    SLOW
};

#endif