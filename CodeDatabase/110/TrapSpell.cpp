#include "TrapSpell.hpp"

TrapSpell::TrapSpell (Field& field, std::vector<unsigned int> stats, size_t range, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target) :
    SpawnSpell(field, range, stats, caster, target) {}

Spell::CastResult TrapSpell::cast () {
    CastResult result;
    if (!canCast(targetX, targetY)) {
        return result;
    }
    result.success = true;

    Trap* spawnedTrap = new Trap (200, stats[0]);
    Cell& targetCell = field.getCell(targetX, targetY);
    targetCell.setEntity(*spawnedTrap);
    result.spawnedEntities.push_back(&targetCell);

    return result;
}

void TrapSpell::upgradeSpell (size_t level) { stats[0] += level * 10; }