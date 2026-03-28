#include "win_condition_checker.h"
#include "game.h"

bool WinConditionChecker::isGameOver(const Game& game) {
    return game.gameOver;
}

bool WinConditionChecker::playerWon(const Game& game) {
    return !game.enemyManager->hasEnemiesRemaining() &&
           game.buildings.empty() &&
           game.towers.empty();
}

bool WinConditionChecker::playerLost(const Game& game) {
    return !game.player.isAlive();
}