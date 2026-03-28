#include "AllySpell.hpp"

AllySpell::AllySpell (Field& field, std::vector<unsigned int> stats, size_t range, unsigned int alliesNumber, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target) :
    SpawnSpell(field, range, stats, caster, target), alliesNumber(alliesNumber) {}

Spell::CastResult AllySpell::cast () {
    CastResult result;

    std::vector<std::pair<size_t, size_t>> spawnPositions;

    if (canCast(targetX, targetY)) {
        spawnPositions.push_back({targetX, targetY});
    }

    for (size_t i = 0; i < alliesNumber - 1; i++) {
        findExtraPositions(spawnPositions);
    }

    if (spawnPositions.empty()) {
        return result;
    }
    result.success = true;
    for (size_t i = 0; i < alliesNumber; i++) {
        auto [x, y] = spawnPositions[i];
        Ally* spawned = new Ally(100, stats[0], stats[0], stats[1]);
        Cell& targetCell = field.getCell(x, y);
        targetCell.setEntity(*spawned);
        result.spawnedEntities.push_back(&targetCell); 
    }

    return result;
}

void AllySpell::upgradeSpell (size_t level) { alliesNumber += level; }

void AllySpell::findExtraPositions (std::vector<std::pair<size_t, size_t>>& spawnPositions) {
    for (size_t y = targetY - range; y < targetY + range; y++) {
        for (size_t x = targetX - range; x < targetX + range; x++) {
            if (canCast(x, y)) {
                spawnPositions.push_back(std::make_pair(x, y));
            }
        }
    }
}