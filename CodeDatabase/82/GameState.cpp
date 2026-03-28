#include "GameState.h"
#include "MovementValidator.h"
#include <algorithm>
#include <stdexcept>

GameState::GameState(int fieldWidth, int fieldHeight)
    : gameField(fieldWidth, fieldHeight),
      player(1, 1),
      isGameRunning(true) {
    addEnemy(Enemy(fieldWidth - 8, fieldHeight - 8));
    addEnemy(Enemy(fieldWidth - 3, fieldHeight - 7));
    addEnemy(Enemy(fieldWidth - 7, fieldHeight - 3));
    addEnemy(Enemy(fieldWidth - 3, fieldHeight - 5));
    addEnemy(Enemy(fieldWidth - 5, fieldHeight - 3));
    
}

std::unique_ptr<GameState> GameState::createNewGame(int fieldWidth, int fieldHeight) {
    auto gameState = std::make_unique<GameState>(fieldWidth, fieldHeight);
    gameState->getPlayer().initializeWithStarterSpell();
    return gameState;
}

std::unique_ptr<GameState> GameState::createEmptyGame(int fieldWidth, int fieldHeight) {
    return std::make_unique<GameState>(fieldWidth, fieldHeight);
}

void GameState::setPlayerPosition(const Position& newPosition) {
    if (MovementValidator::isValidPosition(newPosition, gameField) &&
        MovementValidator::isCellPassable(newPosition, gameField)) {
        for (const auto& enemy : enemies) {
            if (enemy.isAlive() && enemy.getPosition() == newPosition) {
                throw std::runtime_error("Нельзя переместиться в ячейку занятую врагом");
            }
        }
        player.setPosition(newPosition);
    }
}

void GameState::damagePlayer(int damageAmount) {
    player.takeDamage(damageAmount);
    if (!player.isAlive()) {
        isGameRunning = false;
    }
}

void GameState::damageEnemy(int enemyIndex, int damageAmount) {
    if (enemyIndex >= 0 && enemyIndex < static_cast<int>(enemies.size())) {
        enemies[enemyIndex].takeDamage(damageAmount);
    }
}

void GameState::addEnemy(const Enemy& enemy) {
    enemies.push_back(enemy);
}

void GameState::removeDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& enemy) { return !enemy.isAlive(); }),
        enemies.end()
    );
}