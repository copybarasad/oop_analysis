#include "DamagingSpell.h"

std::vector<Coordinates> DamagingSpell::getAttackedCells(Coordinates target_pos) {
    std::vector<Coordinates> attackedCells;
    if (type == Type::AoE) {
        for (int delta_y = 0; delta_y < 2; delta_y++) {
            for (int delta_x = 0; delta_x < 2; delta_x++) {
                attackedCells.push_back(target_pos + Coordinates(delta_x, delta_y));
            }
        }
    }
    else { attackedCells.push_back(target_pos);
    }
    return attackedCells;
}

json DamagingSpell::toJson() const {
    return BasicSpell::toJson();
}

void DamagingSpell::fromJson(const json& j) {
    BasicSpell::fromJson(j);
}