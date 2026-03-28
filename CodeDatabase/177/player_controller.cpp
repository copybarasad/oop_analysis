#include "player_controller.h"

PlayerController::PlayerController(Player& player, GameField& field) 
    : player(player), field(field) {}

bool PlayerController::movePlayer(Direction direction) {
    if (!player.getCanMove()) {
        player.setCanMove(true);
        return false;
    }
    
    Position currentPos = player.getPosition();
    Position directionVec = getDirectionVector(direction);
    Position newPos = currentPos + directionVec;
    
    if (!field.isPositionPassable(newPos)) {
        return false;
    }
    
    if (!field.getCell(newPos).getIsOccupied()) {
        field.setCellOccupied(currentPos, false);
        player.setPosition(newPos);
        field.setCellOccupied(newPos, true);
        
        if (field.getCell(newPos).getType() == CellType::SLOW) {
            player.setCanMove(false);
        }
        return true;
    }
    
    return false;
}

void PlayerController::switchCombatMode() {
    player.switchCombatMode();
}

const Player& PlayerController::getPlayer() const {
    return player;
}

Position PlayerController::getDirectionVector(Direction direction) const {
    switch (direction) {
        case Direction::UP: return Position(0, -1);
        case Direction::DOWN: return Position(0, 1);
        case Direction::LEFT: return Position(-1, 0);
        case Direction::RIGHT: return Position(1, 0);
        default: return Position(0, 0);
    }
}
