#pragma once
#include <vector>

struct Coordinates {
    int x, y;

    bool operator==(const Coordinates& other) const {
        return x == other.x && y == other.y;
    }
};

enum class FieldType {
    PLAYER,
    ENEMY,
    ATTACK_TOWER,
    SPAWN_TOWER,

    FIELD_DEFAULT,
    FIELD_BLOCK,
    FIELD_TRAP,
    FIELD_SLOW
};

enum class SpellType {
    TargetSpell,
    AoESpell,
    TrapSpell
};
