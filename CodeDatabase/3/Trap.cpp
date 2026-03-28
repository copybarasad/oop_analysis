#include "GameObjects/Spells/Trap.hpp"


Trap::Trap(int lvl) {
    this->range = 2 + lvl;
    this->damage = 50 + 25 * lvl;
}


std::vector<Coordinates> Trap::getCastRangeCoordinates(Coordinates castCoord, int mapH, int mapW) {
    std::vector<Coordinates> result;

    int px = castCoord.x;
    int py = castCoord.y;

    result.push_back(castCoord);

    for (int dx = -range; dx <= range; ++dx) {
        for (int dy = -range; dy <= range; ++dy) {
            if (!(dx == 0 && dy == 0)) {
                int x = px + dx;
                int y = py + dy;
                if (x >= 0 && x < mapW && y >= 0 && y < mapH) {
                    result.push_back({x, y});
                }
            }
        }
    }

    return result;
}


bool Trap::canUseHere(FieldType ft) {
    return ft == FieldType::FIELD_DEFAULT;
}

std::vector<SpellContract> Trap::use(Coordinates target) {
    std::vector<SpellContract> res;
    res.push_back(SpellContract{target, this->damage, true});
    return res;
}


SpellType Trap::getSpellType() {
    return SpellType::TrapSpell;
}