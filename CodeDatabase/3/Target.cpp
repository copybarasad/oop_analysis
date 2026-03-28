#include "GameObjects/Spells/Target.hpp"


Target::Target(int lvl) {
    this->damage = 50 + 50 * lvl;
    this->range = 1 + lvl;
}


std::vector<Coordinates> Target::getCastRangeCoordinates(Coordinates castCoord, int mapH, int mapW) {
    std::vector<Coordinates> result;

    int startX = castCoord.x;
    int startY = castCoord.y;

    result.push_back(castCoord);

    for (int dx = -range; dx <= range; ++dx) {
        for (int dy = -range; dy <= range; ++dy) {
            if (std::sqrt(dx * dx + dy * dy) <= range && std::sqrt(dx * dx + dy * dy) > 0) {
                int x = startX + dx;
                int y = startY + dy;
                if (x >= 0 && x < mapW && y >= 0 && y < mapH) {
                    result.push_back({x, y});
                }
            }
        }
    }

    return result;
}

bool Target::canUseHere(FieldType ft) {
    return ft == FieldType::ENEMY || ft == FieldType::ATTACK_TOWER || ft == FieldType::PLAYER;
}

std::vector<SpellContract> Target::use(Coordinates target) {
    std::vector<SpellContract> res;
    res.push_back(SpellContract{target, this->damage, false});
    return res;
}

SpellType Target::getSpellType() {
    return SpellType::TargetSpell;
}