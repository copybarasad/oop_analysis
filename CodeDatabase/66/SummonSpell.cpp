#include "../include/SummonSpell.h"
#include "../include/GameWorld.h"
#include "../include/Ally.h"
#include <vector>

SummonSpell::SummonSpell(int summonCount) 
    : baseSummonCount(summonCount) {}

bool SummonSpell::canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const {
    int enhancements = 0;
    int totalAllies = baseSummonCount + enhancements;
    
    int availableSpaces = 0;
    const MapGrid& grid = world.getMapGrid();
    
    for (int y = 1; y <= grid.getHeight(); y++) {
        for (int x = 1; x <= grid.getWidth(); x++) {
            Position pos(x, y);
            if (grid.canMoveTo(pos) && 
                !world.hasAliveEnemyAt(pos) &&
                !world.isPositionOccupiedByAlly(pos) &&
                !world.hasBuildingAt(pos) &&
                !world.hasTowerAt(pos) &&
                !world.hasTrapAt(pos) &&
                pos != playerPos) {
                availableSpaces++;
                if (availableSpaces >= totalAllies) {
                    return true;
                }
            }
        }
    }
    
    return availableSpaces > 0;
}

void SummonSpell::cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) {
    int enhancements = buffer.getEnhancementCount(SpellType::SUMMON);
    int totalAllies = baseSummonCount + enhancements;
    
    std::vector<Position> availablePositions;
    const MapGrid& grid = world.getMapGrid();
    
    for (int y = 1; y <= grid.getHeight(); y++) {
        for (int x = 1; x <= grid.getWidth(); x++) {
            Position pos(x, y);
            if (grid.canMoveTo(pos) && 
                !world.hasAliveEnemyAt(pos) &&
                !world.isPositionOccupiedByAlly(pos) &&
                !world.hasBuildingAt(pos) &&
                !world.hasTowerAt(pos) &&
                !world.hasTrapAt(pos) &&
                pos != playerPos) {
                availablePositions.push_back(pos);
            }
        }
    }
    
    int alliesSpawned = 0;
    for (size_t i = 0; i < availablePositions.size() && alliesSpawned < totalAllies; i++) {
        world.getAllyManager().addAlly(availablePositions[i], std::make_unique<Ally>(30, 15));
        alliesSpawned++;
    }
}

SpellType SummonSpell::getType() const {
    return SpellType::SUMMON;
}

std::string SummonSpell::getName() const {
    return "Summon";
}

std::string SummonSpell::getDescription(const EnhancementBuffer& buffer) const {
    int enhancements = buffer.getEnhancementCount(SpellType::SUMMON);
    int totalAllies = baseSummonCount + enhancements;
    return "Summon " + std::to_string(totalAllies) + " ally(s) (30 HP, 15 dmg each)";
}

int SummonSpell::getBaseSummonCount() const {
    return baseSummonCount;
}
