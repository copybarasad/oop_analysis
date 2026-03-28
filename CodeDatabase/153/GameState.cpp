#include "GameState.h"
#include "GameConstants.h"
#include <stdexcept>

GameState::GameState()
    : field(GameConstants::FIELD_WIDTH, GameConstants::FIELD_HEIGHT) {
}

void GameState::initializeNewGame() {
    enemies.clear();
    field.clearAllCells();
    field.setPlayerPosition(0, 0);

    player.setHealth(GameConstants::PLAYER_START_HEALTH);
    player.setMana(GameConstants::PLAYER_START_MANA);
    player.setMaxMana(GameConstants::PLAYER_MAX_MANA);
    player.setScore(GameConstants::PLAYER_START_SCORE);

    enemies.push_back(Enemy(5, 5));
    enemies.push_back(Enemy(10, 10));

    resetTurns();
}

void GameState::initializeTestGame() {
    initializeNewGame();
}

bool GameState::checkPlayerVictory() const {
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            return false;
        }
    }
    return true;
}

bool GameState::isValidMove(int newX, int newY) const {
    if (!field.isValidPosition(newX, newY)) {
        return false;
    }

    // Проверяем, не занята ли клетка врагом (для атаки)
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == newX && enemy.getY() == newY) {
            return true; // Можно атаковать
        }
    }

    // Проверяем, пуста ли клетка для движения
    return field.isCellEmpty(newX, newY);
}

bool GameState::canPlayerCastSpell(int spellIndex) const {
    if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSpellCount()) {
        return false;
    }
    int manaCost = player.getSpellHand().getSpellManaCost(spellIndex);
    return player.getMana() >= manaCost;
}