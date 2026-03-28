#include "game_state.h"
#include <iostream>

GameState::GameState(int fieldWidth, int fieldHeight, int level)
    : currentLevel(level),
      totalScore(0),
      field(fieldWidth, fieldHeight),
      player(fieldWidth / 2, fieldHeight / 2),
      enemyBuilding(0, 0),
      enemyTower(0, 0),
      enemiesDefeated(0) {
}

int GameState::getCurrentLevel() const {
    return currentLevel;
}

int GameState::getTotalScore() const {
    return totalScore;
}

GameField& GameState::getField() {
    return field;
}

const GameField& GameState::getField() const {
    return field;
}

Player& GameState::getPlayer() {
    return player;
}

const Player& GameState::getPlayer() const {
    return player;
}

std::vector<Enemy>& GameState::getEnemies() {
    return enemies;
}

const std::vector<Enemy>& GameState::getEnemies() const {
    return enemies;
}

EnemyBuilding& GameState::getEnemyBuilding() {
    return enemyBuilding;
}

const EnemyBuilding& GameState::getEnemyBuilding() const {
    return enemyBuilding;
}

EnemyTower& GameState::getEnemyTower() {
    return enemyTower;
}

const EnemyTower& GameState::getEnemyTower() const {
    return enemyTower;
}

std::vector<Trap>& GameState::getTraps() {
    return traps;
}

const std::vector<Trap>& GameState::getTraps() const {
    return traps;
}

int GameState::getEnemiesDefeated() const {
    return enemiesDefeated;
}

void GameState::setCurrentLevel(int level) {
    currentLevel = level;
}

void GameState::setTotalScore(int score) {
    totalScore = score;
}

void GameState::setEnemiesDefeated(int count) {
    enemiesDefeated = count;
}

void GameState::resetForNewLevel(int width, int height) {
    int savedMaxHealth = player.getMaxHealth();
    int savedHealth = player.getHealth();
    int savedMeleeDamage = player.getMeleeDamage();
    int savedRangedDamage = player.getRangedDamage();
    int savedScore = player.getScore();
    CombatMode savedCombatMode = player.getCombatMode();
    auto savedSpellHand = std::move(player.getSpellHand());

    field = GameField(width, height);
    
    player = Player(width / 2, height / 2);
    
    player.increaseMaxHealth(savedMaxHealth - player.getMaxHealth());
    player.increaseMeleeDamage(savedMeleeDamage - player.getMeleeDamage());
    player.increaseRangedDamage(savedRangedDamage - player.getRangedDamage());
    player.addScore(savedScore);
    
    if (savedCombatMode != player.getCombatMode()) {
        player.switchCombatMode();
    }
    
    player.setHealth(savedHealth);
    player.getSpellHand() = std::move(savedSpellHand);

    enemies.clear();
    traps.clear();
    enemyBuilding = EnemyBuilding(-1, -1);
    enemyBuilding.setHealth(0);
    enemyTower = EnemyTower(-1, -1);
    enemyTower.setHealth(0);
    
    enemiesDefeated = 0;
}

bool GameState::isLevelCompleted() const {
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            return false;
        }
    }
    return !enemyBuilding.isAlive() && !enemyTower.isAlive();
}

bool GameState::isGameOver() const {
    return !player.isAlive();
}