#include "Spell.h"
#include "GameState.h"
#include "MovementValidator.h"

bool Spell::canCast(const Position& casterPos, const Position& target, const GameState& gameState) const {
    if (!MovementValidator::isValidPosition(target, gameState.getGameField())) {
        return false;
    }

    int distance = std::abs(casterPos.x - target.x) + std::abs(casterPos.y - target.y);
    return distance <= getRange();
}

int Spell::calculateDistance(const Position& casterPos, const Position& target) const {
    return std::abs(casterPos.x - target.x) + std::abs(casterPos.y - target.y);
}