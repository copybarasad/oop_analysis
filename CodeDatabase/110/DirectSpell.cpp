#include "DirectSpell.hpp"

DirectSpell::DirectSpell (Field& field, unsigned int damage, size_t range, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target) :
    DamageSpell(field, range, damage, caster, target) {}

Spell::CastResult DirectSpell::cast () {
    CastResult result;
    if (!canCast(targetX, targetY)) {
        return result;
    }
    result.success = true;
    dealDamage(result, targetX, targetY);
    return result;
}

void DirectSpell::upgradeSpell (size_t level) { range += level * 3; }

bool DirectSpell::canCast (size_t targetX, size_t targetY) const {
    if (targetX >= field.getWidth() || targetY >= field.getHeight()) {
        return false;
    }

    size_t distance = calculateDistance(targetX, targetY);
    if (distance > range || field.getCell(targetX, targetY).isEmpty()) {
        return false;
    }
    return true;
}