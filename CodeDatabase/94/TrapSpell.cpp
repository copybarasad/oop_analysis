#include "TrapSpell.h"
#include "GameField.h"
#include "Trap.h"
#include "Player.h"
#include <cmath>

bool TrapSpell::cast(const Position& target, GameField& field, Character& caster) {
    Player* player = dynamic_cast<Player*>(&caster);
    if (!player) return false;
    
    Position casterPos = player->getPosition();
    
    int distance = std::abs(target.x() - casterPos.x()) + std::abs(target.y() - casterPos.y());
    if (distance > getRange()) {
        return false;
    }
    
    if (!field.isValidPosition(target) || !field.isPositionPassable(target)) {
        return false;
    }
    
    if (field.hasCharacterAt(target) || field.hasTrapAt(target) || field.hasTowerAt(target)) {
        return false;
    }
    
    field.placeTrap(target);
    return true;
}