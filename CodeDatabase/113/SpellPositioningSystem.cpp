#include "SpellPositioningSystem.h"
#include "Player.h"
#include "GameField.h"
#include <cmath>

bool SpellPositioningSystem::isValidSpellPosition(const Player& caster, const GameField& field, int x, int y, int range) {
    int distance = calculateDistance(caster, x, y);
    return distance <= range &&
        field.isValidPosition(x, y) &&
        field.isCellPassable(x, y);
}

std::vector<std::pair<int, int>> SpellPositioningSystem::getValidSpellPositions(const Player& caster, const GameField& field, int range, bool excludePlayerPosition) {
    std::vector<std::pair<int, int>> validPositions;
    int playerX = caster.getPositionX();
    int playerY = caster.getPositionY();

    for (int x = 0; x < field.getWidth(); ++x) {
        for (int y = 0; y < field.getHeight(); ++y) {
            if (excludePlayerPosition && x == playerX && y == playerY) {
                continue;
            }
            if (isValidSpellPosition(caster, field, x, y, range)) {
                validPositions.push_back({ x, y });
            }
        }
    }
    return validPositions;
}

int SpellPositioningSystem::calculateDistance(const Player& caster, int x, int y) {
    return std::abs(x - caster.getPositionX()) + std::abs(y - caster.getPositionY());
}