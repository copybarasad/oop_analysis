#include "Game.h"
#include "GameConstants.h"
#include "LevelManager.h"
#include "UpgradeSystem.h"
#include <algorithm>
#include <cstdlib>

void Game::initializeLevel(int level) {
    placePlayerInSafeLocation();
    placeEnemiesInSafeLocations(level);
    placeBuildingsInSafeLocations(level);
    placeTowersInSafeLocations(level);
    addRandomSpellToPlayer();

    int scoreTarget = LevelManager::getScoreTarget(level);
    addMessage("Уровень " + std::to_string(level) + " создан: зданий " +
               std::to_string(buildings.size()) + ", башен " +
               std::to_string(towers.size()) + ", врагов " +
               std::to_string(enemies.size()) + ". Нужно " +
               std::to_string(scoreTarget) + " очков для прохождения.");
}

void Game::handleLevelComplete() {
    addMessage("Уровень " + std::to_string(currentLevel) + " завершён");
    render();

    int playerHealth = player.getHealth();
    int maxHealth = GameConstants::PLAYER_INITIAL_HEALTH;
    player.setHealth(std::min(playerHealth + maxHealth / 2, maxHealth));
    player.setScore(0);

    Hand &hand = player.getHand();
    int handSize = hand.getSize();
    int cardsToRemove = handSize / 2;
    for (int i = 0; i < cardsToRemove && !hand.isEmpty(); ++i) {
        int randomIndex = std::rand() % hand.getSize();
        hand.removeSpell(randomIndex);
    }

    if (currentLevel >= 3) {
        addMessage("Пройдены все 3 уровня. Игра успешно завершена.");
        render();
        gameRunning = false;
        return;
    }

    handleUpgradeMenu();

    currentLevel++;
    setEnemiesKilled(0);
    LevelManager::initializeLevel(*this, currentLevel);
    render();
}

void Game::handleUpgradeMenu() {
    auto options = UpgradeSystem::generateUpgradeOptions(player);
    if (options.empty()) {
        addMessage("Нет доступных улучшений.");
        return;
    }
    int choiceIndex = 0;
    if (upgradeSelector) {
        int idx = upgradeSelector(options);
        if (idx >= 0 && idx < static_cast<int>(options.size())) {
            choiceIndex = idx;
        }
    }
    UpgradeSystem::applyUpgrade(player, player.getHand(), options[choiceIndex]);
    addMessage("Улучшение применено: " + options[choiceIndex].description);
}

bool Game::findSafeSpawnPosition(int &x, int &y, int maxAttempts, int safeRadius) const {
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        x = std::rand() % field.getWidth();
        y = std::rand() % field.getHeight();
        if (isValidSpawnPosition(x, y, safeRadius)) return true;
    }

    for (int y_pos = 0; y_pos < field.getHeight(); ++y_pos) {
        for (int x_pos = 0; x_pos < field.getWidth(); ++x_pos) {
            if (isValidSpawnPosition(x_pos, y_pos, safeRadius)) {
                x = x_pos;
                y = y_pos;
                return true;
            }
        }
    }
    return false;
}

bool Game::isValidSpawnPosition(int x, int y, int safeRadius) const {
    if (!field.isValidPosition(x, y) || !field.isPassable(x, y) ||
        !movementSystem.isPositionFree(x, y, field, player, enemies, buildings, towers, allies)) {
        return false;
    }

    if (safeRadius > 0) {
        for (int dy = -safeRadius; dy <= safeRadius; ++dy) {
            for (int dx = -safeRadius; dx <= safeRadius; ++dx) {
                int checkX = x + dx;
                int checkY = y + dy;
                if (field.isValidPosition(checkX, checkY) &&
                    !movementSystem.isPositionFree(checkX, checkY, field, player, enemies, buildings, towers, allies)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Game::placePlayerInSafeLocation() {
    int playerX, playerY;
    if (findSafeSpawnPosition(playerX, playerY)) {
        player.setPosition(playerX, playerY);
        addMessage("Игрок появился в (" + std::to_string(playerX) + ", " + std::to_string(playerY) + ")");
    } else {
        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                if (field.isPassable(x, y) &&
                    movementSystem.isPositionFree(x, y, field, player, enemies, buildings, towers, allies)) {
                    player.setPosition(x, y);
                    addMessage("Аварийно: игрок помещён в (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                    return;
                }
            }
        }
        throw std::runtime_error("Не удалось найти позицию для игрока");
    }
}

void Game::placeEnemiesInSafeLocations() {
    placeEnemiesInSafeLocations(currentLevel);
}

void Game::placeEnemiesInSafeLocations(int level) {
    auto enemySpawner = [this, level](std::vector<Enemy> &cont, int x, int y) {
        cont.push_back(createRandomEnemy(x, y, level));
    };

    int enemyCount = LevelManager::calculateEnemyCount(level);
    int enemySafeRadius = (level == 1) ? 3 : GameConstants::SPAWN_SAFE_RADIUS;
    int enemiesPlaced = spawnObjects(enemies, enemyCount,
                                     GameConstants::ENEMY_SPAWN_ATTEMPTS, "Враг", enemySpawner, enemySafeRadius);

    showSpawnWarning(enemiesPlaced, enemyCount, "врагов");
}

void Game::placeBuildingsInSafeLocations() {
    placeBuildingsInSafeLocations(currentLevel);
}

void Game::placeBuildingsInSafeLocations(int level) {
    auto buildingSpawner = [this](std::vector<EnemyBuilding> &cont, int x, int y) {
        int cooldown = GameConstants::BUILDING_MIN_COOLDOWN +
                       (std::rand() % (GameConstants::BUILDING_MAX_COOLDOWN - GameConstants::BUILDING_MIN_COOLDOWN +
                                       1));
        cont.emplace_back(x, y, cooldown, field.getWidth(), field.getHeight());
    };

    int buildingCount = LevelManager::calculateBuildingCount(level);
    int buildingSafeRadius = (level == 1) ? 2 : GameConstants::SPAWN_SAFE_RADIUS;
    int buildingsPlaced = spawnObjects(buildings, buildingCount,
                                       GameConstants::BUILDING_SPAWN_ATTEMPTS, "Вражеское здание",
                                       buildingSpawner, buildingSafeRadius);

    showSpawnWarning(buildingsPlaced, buildingCount, "зданий");
}

void Game::placeTowersInSafeLocations() {
    placeTowersInSafeLocations(currentLevel);
}

void Game::placeTowersInSafeLocations(int level) {
    auto towerSpawner = [this](std::vector<EnemyTower> &cont, int x, int y) {
        cont.emplace_back(x, y, GameConstants::TOWER_ATTACK_RADIUS,
                          GameConstants::TOWER_DAMAGE, GameConstants::TOWER_COOLDOWN,
                          field.getWidth(), field.getHeight());
    };

    int towerCount = LevelManager::calculateTowerCount(level);
    int towerSafeRadius = (level == 1) ? 2 : GameConstants::SPAWN_SAFE_RADIUS;
    int towersPlaced = spawnObjects(towers, towerCount,
                                    GameConstants::TOWER_SPAWN_ATTEMPTS, "Вражеская башня",
                                    towerSpawner, towerSafeRadius);

    showSpawnWarning(towersPlaced, towerCount, "башен");
}

Enemy Game::createRandomEnemy(int x, int y) {
    return createRandomEnemy(x, y, currentLevel);
}

Enemy Game::createRandomEnemy(int x, int y, int level) {
    int minDamage = LevelManager::calculateEnemyMinDamage(level);
    int maxDamage = LevelManager::calculateEnemyMaxDamage(level);
    int damage = minDamage + (std::rand() % (maxDamage - minDamage + 1));
    int health = LevelManager::calculateEnemyHealth(level);

    return Enemy(x, y, health, damage, field.getWidth(), field.getHeight());
}

template<typename T, typename SpawnFunc>
int Game::spawnObjects(std::vector<T> &container, int desiredCount, int maxAttempts,
                       const std::string &objectName, SpawnFunc spawnFunc, int safeRadius) {
    int objectsPlaced = 0;
    for (int attempt = 0; attempt < maxAttempts && objectsPlaced < desiredCount; ++attempt) {
        int x, y;
        if (findSafeSpawnPosition(x, y, maxAttempts, safeRadius)) {
            spawnFunc(container, x, y);
            objectsPlaced++;
            addMessage(objectName + " создан в (" + std::to_string(x) + ", " + std::to_string(y) + ")");
        }
    }
    return objectsPlaced;
}

void Game::showSpawnWarning(int placed, int expected, const std::string &objectType) {
    if (placed < expected) {
        addMessage("Предупреждение: размещено " + std::to_string(placed) + " " + objectType +
                   " вместо " + std::to_string(expected));
    }
}
