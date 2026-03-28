#include "AreaSpell.hpp"

AreaSpell::AreaSpell (Field& field, unsigned int damage, size_t range, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target) :
    DamageSpell(field, range, damage, caster, target) {}

Spell::CastResult AreaSpell::cast () {
    CastResult result;
    if (!canCast(targetX, targetY)) {
        return result;
    }
    result.success = true;
    for (size_t y = targetY - 1; y <= targetY + 1; y++) {
        for (size_t x = targetX - 1; x <= targetX + 1; x++) {
            if (canSpread(x, y)) {
                dealDamage(result, x, y);
            }
        }
    }
    return result;
}

void AreaSpell::upgradeSpell (size_t level) { range += level * 1; }

bool AreaSpell::canCast (size_t targetX, size_t targetY) const {
    if (targetX >= field.getWidth() || targetY >= field.getHeight()) {
        return false;
    }
    size_t distance = calculateDistance(targetX, targetY);
    if (distance > range) {
        return false;
    }
    return true;
}

bool AreaSpell::canSpread (size_t x, size_t y) const {
    return !field.getCell(x, y).isEmpty();
}