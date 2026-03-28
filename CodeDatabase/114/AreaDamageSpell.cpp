#include "AreaDamageSpell.h"
#include "Player.h"
#include <algorithm>
#include <cmath>

#define AREA_DAMAGE 50

template<typename T>
void applyAreaDamage(int pendingBoost, Entity& center, std::vector<T>& entities) {
    const int radius = std::max(1, pendingBoost);
    for (auto& candidate : entities) {
        int dx = std::abs(candidate.getX() - center.getX());
        int dy = std::abs(candidate.getY() - center.getY());
        if (dx <= radius && dy <= radius) {
            candidate.takeDamage(AREA_DAMAGE);
        }
    }
}

bool AreaDamageSpell::cast(int pendingBoost, Entity* target, ISpellField* field) {
    if (!target) {
        return false;
    }
    applyAreaDamage(pendingBoost, *target, field->getEnemies());
    applyAreaDamage(pendingBoost, *target, field->getBuilding());
    return true;
}
