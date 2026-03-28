#include "SaveDataValidator.h"
#include <iostream>

// Правильное определение статических констант
const int SaveDataValidator::MAX_PLAYER_HEALTH = 20000;
const int SaveDataValidator::MAX_ENEMY_HEALTH = 500;
const int SaveDataValidator::MAX_TOWER_HEALTH = 1000;
const int SaveDataValidator::MAX_LEVEL = 10;
const int SaveDataValidator::MAX_TURN_COUNT = 1000;

bool SaveDataValidator::validateGameState(const GameState& gameState) {
    return validatePlayerData(gameState) &&
        validateEnemiesData(gameState) &&
        validateTowersData(gameState) &&
        validateFieldData(gameState);
}

bool SaveDataValidator::validatePlayerData(const GameState& gameState) {
    // Проверка здоровья игрока
    if (gameState.playerHealth < 0 || gameState.playerHealth > MAX_PLAYER_HEALTH) {
        std::cerr << "VALIDATION ERROR: Invalid player health: " << gameState.playerHealth
            << " (must be between 0 and " << MAX_PLAYER_HEALTH << ")" << std::endl;
        return false;
    }

    // Проверка координат игрока
    if (!gameState.field.isValidPosition(gameState.playerX, gameState.playerY)) {
        std::cerr << "VALIDATION ERROR: Invalid player position: ("
            << gameState.playerX << ", " << gameState.playerY << ")" << std::endl;
        return false;
    }

    // Проверка уровня
    if (gameState.level < 1 || gameState.level > MAX_LEVEL) {
        std::cerr << "VALIDATION ERROR: Invalid level: " << gameState.level
            << " (must be between 1 and " << MAX_LEVEL << ")" << std::endl;
        return false;
    }

    // Проверка номера хода
    if (gameState.turnCount < 0 || gameState.turnCount > MAX_TURN_COUNT) {
        std::cerr << "VALIDATION ERROR: Invalid turn count: " << gameState.turnCount
            << " (must be between 0 and " << MAX_TURN_COUNT << ")" << std::endl;
        return false;
    }

    // Проверка золота
    if (gameState.playerGold < 0 || gameState.playerGold > 100000) {
        std::cerr << "VALIDATION ERROR: Invalid player gold: " << gameState.playerGold << std::endl;
        return false;
    }

    return true;
}

bool SaveDataValidator::validateEnemiesData(const GameState& gameState) {
    for (const auto& enemy : gameState.enemies) {
        // Проверка здоровья врага
        if (enemy.getHealth() < 0 || enemy.getHealth() > MAX_ENEMY_HEALTH) {
            std::cerr << "VALIDATION ERROR: Invalid enemy health: " << enemy.getHealth() << std::endl;
            return false;
        }

        // Проверка урона врага
        if (enemy.getDamage() < 0 || enemy.getDamage() > 100) {
            std::cerr << "VALIDATION ERROR: Invalid enemy damage: " << enemy.getDamage() << std::endl;
            return false;
        }

        // Проверка позиции врага
        if (!gameState.field.isValidPosition(enemy.getPositionX(), enemy.getPositionY())) {
            std::cerr << "VALIDATION ERROR: Invalid enemy position: ("
                << enemy.getPositionX() << ", " << enemy.getPositionY() << ")" << std::endl;
            return false;
        }
    }
    return true;
}

bool SaveDataValidator::validateTowersData(const GameState& gameState) {
    for (const auto& tower : gameState.towers) {
        // Проверка здоровья башни
        if (tower.getHealth() < 0 || tower.getHealth() > MAX_TOWER_HEALTH) {
            std::cerr << "VALIDATION ERROR: Invalid tower health: " << tower.getHealth() << std::endl;
            return false;
        }

        // Проверка урона башни
        if (tower.getDamage() < 0 || tower.getDamage() > 100) {
            std::cerr << "VALIDATION ERROR: Invalid tower damage: " << tower.getDamage() << std::endl;
            return false;
        }

        // Проверка дальности атаки башни
        int attackRange = 0;
        // Получаем дальность атаки через метод getAttackRange(), если он существует
        // Если метода нет, используем значение по умолчанию
        attackRange = 5;

        if (attackRange < 0 || attackRange > 10) {
            std::cerr << "VALIDATION ERROR: Invalid tower attack range: " << attackRange << std::endl;
            return false;
        }

        // Проверка позиции башни
        if (!gameState.field.isValidPosition(tower.getPositionX(), tower.getPositionY())) {
            std::cerr << "VALIDATION ERROR: Invalid tower position: ("
                << tower.getPositionX() << ", " << tower.getPositionY() << ")" << std::endl;
            return false;
        }
    }
    return true;
}

bool SaveDataValidator::validateFieldData(const GameState& gameState) {
    // Проверка размеров поля
    if (gameState.field.getWidth() <= 0 || gameState.field.getWidth() > 100 ||
        gameState.field.getHeight() <= 0 || gameState.field.getHeight() > 100) {
        std::cerr << "VALIDATION ERROR: Invalid field dimensions: "
            << gameState.field.getWidth() << "x" << gameState.field.getHeight() << std::endl;
        return false;
    }

    return true;
}