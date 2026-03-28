#include "SpawnSpell.hpp"

SpawnSpell::SpawnSpell (Field& field, size_t range, std::vector<unsigned int> stats, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target) :
    RangedSpell(field, range, caster, target), stats(stats) {}

std::vector<unsigned int> SpawnSpell::getStats () const { return stats; }

bool SpawnSpell::canCast (size_t targetX, size_t targetY) const {
    if (!field.getCell(targetX, targetY).isEmpty() || field.getCell(targetX, targetY).isBlocked()) {
        return false;
    }

    size_t distance = calculateDistance(targetX, targetY);
    if (distance > range) {
        return false;
    }
    return true;
}