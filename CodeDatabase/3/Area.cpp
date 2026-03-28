#include "GameObjects/Spells/Area.hpp"


Area::Area(int lvl) {
    this->damage = 25 + 25 * lvl;
    this->range = 3 + lvl;
}


std::vector<Coordinates> Area::getCastRangeCoordinates(Coordinates castCoord, int mapH, int mapW) {
    std::vector<Coordinates> result;

    int startX = castCoord.x;
    int startY = castCoord.y;

    result.push_back(castCoord);

    for (int dx = -range; dx <= range; ++dx) {
        for (int dy = -range; dy <= range; ++dy) {
            int distance = std::abs(dx) + std::abs(dy);
            if (distance <= range && distance > 0) {
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


bool Area::canUseHere(FieldType ft) {
    return true;
}

std::vector<SpellContract> Area::use(Coordinates target) {
    std::vector<SpellContract> res;

    std::vector<Coordinates> area;
    for (int dx = 0; dx < 2; ++dx) {
        for (int dy = 0; dy < 2; ++dy) {
            area.push_back({target.x + dx, target.y + dy});
        }
    }

    for (const Coordinates& c : area) {
        res.push_back(SpellContract{c, this->damage, false});
    }
    return res;
}


SpellType Area::getSpellType() {
    return SpellType::AoESpell;
}