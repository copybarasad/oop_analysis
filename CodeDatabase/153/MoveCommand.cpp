#include "MoveCommand.h"
#include "GameState.h"
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "GameConstants.h"
#include <iostream>

MoveCommand::MoveCommand(int deltaX, int deltaY)
    : dx(deltaX), dy(deltaY) {
}

bool MoveCommand::execute(GameState& gameState) {
    auto playerPos = gameState.getField().getPlayerPosition();
    int newX = playerPos.first + dx;
    int newY = playerPos.second + dy;

    auto& field = gameState.getField();
    if (!field.isValidPosition(newX, newY)) {
        return false;
    }

    auto& enemies = gameState.getEnemies();
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == newX && enemy.getY() == newY) {
            enemy.takeDamage(gameState.getPlayer().getDamage());
            if (!enemy.isAlive()) {
                gameState.getPlayer().addScore(GameConstants::SCORE_FOR_KILL);
            }
            return true;
        }
    }

    if (field.isCellEmpty(newX, newY)) {
        field.movePlayer(newX, newY);
        gameState.getPlayer().addScore(GameConstants::SCORE_FOR_MOVE);
        gameState.getPlayer().addMana(GameConstants::MANA_FOR_MOVE);
        return true;
    }
    return false;
}

std::string MoveCommand::getName() const {
    if (dx == 0 && dy == -1) return "MoveCommand(UP)";
    if (dx == 0 && dy == 1)  return "MoveCommand(DOWN)";
    if (dx == -1 && dy == 0) return "MoveCommand(LEFT)";
    if (dx == 1 && dy == 0)  return "MoveCommand(RIGHT)";
    return "MoveCommand(UNKNOWN)";
}