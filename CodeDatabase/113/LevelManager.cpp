#include "LevelManager.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <iostream>

LevelManager::LevelManager()
    : m_obstacleGenerator(std::make_unique<ObstacleGenerator>())
    , m_currentLevel(1) {
}

bool LevelManager::initializeLevel(GameState& gameState, int level) {
    m_currentLevel = level;
    gameState.level = level;
    gameState.turnCount = 0;

    // Очищаем предыдущее состояние
    gameState.enemies.clear();
    gameState.towers.clear();

    // Настраиваем врагов и башни
    setupEnemiesForLevel(gameState, level);
    setupTowersForLevel(gameState, level);

    // Генерируем препятствия
    m_obstacleGenerator->generateObstacles(gameState.field, gameState.enemies, gameState.towers, level);

    std::cout << "Level " << level << " initialized with "
        << gameState.enemies.size() << " enemies and "
        << gameState.towers.size() << " towers" << std::endl;
    return true;
}

bool LevelManager::advanceToNextLevel(GameState& gameState, Player& player) {
    int nextLevel = m_currentLevel + 1;
    if (initializeLevel(gameState, nextLevel)) {
        applyLevelBonuses(player, nextLevel);

        // Обновляем ссылки игрока
        player.setEnemiesReference(gameState.enemies);
        player.setTowersReference(gameState.towers);

        std::cout << "\n*** WELCOME TO LEVEL " << nextLevel << "! ***" << std::endl;
        std::cout << "Enemies are stronger now! Be careful!" << std::endl;
        return true;
    }
    return false;
}

void LevelManager::setupEnemiesForLevel(GameState& gameState, int level) {
    int enemyHealth = 50 + (level - 1) * 20;
    int enemyDamage = 5 + (level - 1) * 2;
    int enemyCount = 2 + level;

    if (enemyCount > 8) enemyCount = 8;

    // Размещаем врагов в разных позициях в зависимости от уровня
    std::vector<std::pair<int, int>> positions = {
        {3, 3}, {12, 12}, {3, 12}, {12, 3},
        {7, 7}, {7, 10}, {10, 7}, {10, 10}
    };

    for (int i = 0; i < enemyCount && i < positions.size(); ++i) {
        const auto& pos = positions[i];
        gameState.enemies.emplace_back(enemyHealth, enemyDamage, pos.first, pos.second);
    }
}

void LevelManager::setupTowersForLevel(GameState& gameState, int level) {
    int towerHealth = 100 + (level - 1) * 30;
    int towerDamage = 10 + (level - 1) * 3;
    int towerCount = 2 + (level / 2);

    if (towerCount > 4) towerCount = 4;

    std::vector<std::pair<int, int>> positions = {
        {5, 5}, {10, 10}, {5, 10}, {10, 5}
    };

    for (int i = 0; i < towerCount && i < positions.size(); ++i) {
        const auto& pos = positions[i];
        gameState.towers.emplace_back(towerHealth, towerDamage, 5, pos.first, pos.second);
    }
}

void LevelManager::applyLevelBonuses(Player& player, int level) {
    player.setHealth(player.getHealth() + 500);
    player.addMana(100);
    player.addScore(500 * level);

    std::cout << "Bonus: +500 HP, +100 Mana, +" << (500 * level) << " Score" << std::endl;
}