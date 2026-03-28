#ifndef SPELL_SAVE_DATA_H
#define SPELL_SAVE_DATA_H

#include <string>

enum class SpellType {
    DIRECT_DAMAGE,
    AREA_DAMAGE,
    TRAP,
    SUMMON,
    UPGRADE
};

struct SpellSaveData {
    SpellType type;
    int damage;
    int range;
    int width;
    int height;
    int count;
    int power;
    bool improved;
    std::string name;
};

#endif