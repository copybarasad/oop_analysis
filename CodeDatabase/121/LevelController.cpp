#include "LevelController.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SpellFactory.h"
#include "GameExceptions.h"
#include <cstdlib>
#include <ctime>
#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>

// Основной конструктор уровня с конфигурацией
LevelController::LevelController(int number, LevelType levelType, const GameConfig& gameConfig) 
    : levelNumber(number), 
      type(levelType), 
      config(gameConfig),
      field(15, 15, {0, 0}),
      player({0, 0}), 
      aiSystem(),
      maxEnemies(10) {
    
    try {
        initialize();
    } catch (const LevelGenerationException& e) {
        // Перебрасываем с информацией об уровне
        throw LevelGenerationException("Уровень " + std::to_string(number) + ": " + e.what());
    }
}

// Конструктор копирования
LevelController::LevelController(const LevelController& other)
    : levelNumber(other.levelNumber), 
      type(other.type), 
      config(other.config),
      field(other.field), 
      player(Position(0, 0)),
      aiSystem(),
      maxEnemies(other.maxEnemies) {
    
    try {
        // Копируем базовые параметры игрока
        player.setHealth(other.player.getHealth());
        player.setScore(other.player.getScore());
        player.setDamage(other.player.getDamage());
        player.setSlowed(other.player.getSlowed());
        player.setCanAct(other.player.getCanAct());
        player.setBattleMode(other.player.getBattleMode());
        player.setPosition(other.player.getPosition());
        
        // Копируем врагов
        for (const auto& enemy : other.enemies) {
            if (enemy.isAlive()) {
                Enemy newEnemy(enemy.getPosition(), config.getEnemyConfig());
                newEnemy.setHealth(enemy.getHealth());
                enemies.push_back(newEnemy);
            }
        }
        
        // Копируем здания
        for (const auto& building : other.buildings) {
            if (building.isAlive()) {
                EnemyBuilding newBuilding(building.getPosition(), config.getBuildingConfig());
                newBuilding.setHealth(building.getHealth());
                buildings.push_back(newBuilding);
            }
        }
        
        // Копируем башни
        for (const auto& tower : other.towers) {
            if (tower.isAlive()) {
                EnemyTower newTower(tower.getPosition(), config.getTowerConfig());
                newTower.setHealth(tower.getHealth());
                towers.push_back(newTower);
            }
        }
        
        // Копируем заклинания
        auto& otherHand = other.player.getHand();
        auto& hand = player.getHand();
        
        // Очищаем текущую руку
        while (hand.getSpellCount() > 0) {
            hand.removeSpell(0);
        }
        
        // Копируем заклинания из другой руки
        for (size_t i = 0; i < otherHand.getSpellCount(); i++) {
            if (const Spell* spell = otherHand.getSpell(i)) {
                if (dynamic_cast<const DirectDamageSpell*>(spell)) {
                    hand.addSpell(std::make_shared<DirectDamageSpell>());
                } else if (dynamic_cast<const AreaDamageSpell*>(spell)) {
                    hand.addSpell(std::make_shared<AreaDamageSpell>());
                } else if (dynamic_cast<const TrapSpell*>(spell)) {
                    hand.addSpell(std::make_shared<TrapSpell>());
                }
            }
        }
        
    } catch (const std::exception& e) {
        throw LevelGenerationException("Ошибка копирования уровня: " + std::string(e.what()));
    }
}

// Инициализация уровня
void LevelController::initialize() {
    try {
        generateLevel();
        giveStartingSpell();
    } catch (const std::exception& e) {
        throw LevelGenerationException("Ошибка инициализации: " + std::string(e.what()));
    }
}

// Генерация уровня в зависимости от типа
void LevelController::generateLevel() {
    cleanup();
    
    try {
        // Получаем конфигурацию уровня из общей конфигурации
        const LevelConfig& levelConfig = config.getLevelConfig(type);
        
        // Создаем поле с размерами из конфигурации
        field = GameField(levelConfig.width, levelConfig.height, {0, 0});
        maxEnemies = levelConfig.width * levelConfig.height * levelConfig.enemyDensity;
        
        placePlayer();
        
        switch (type) {
            case LevelType::FOREST:
                generateForestLevel();
                break;
            case LevelType::CASTLE:
                generateCastleLevel();
                break;
            case LevelType::DUNGEON:
                generateDungeonLevel();
                break;
        }
    } catch (const FieldException& e) {
        throw LevelGenerationException("Ошибка создания поля: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw LevelGenerationException("Ошибка генерации уровня: " + std::string(e.what()));
    }
}

// Генерация лесного уровня (легкий)
void LevelController::generateForestLevel() {
    const LevelConfig& levelConfig = config.getLevelConfig(LevelType::FOREST);
    
    try {
        for (int i = 0; i < levelConfig.enemyCount; i++) {
            placeEnemy();
        }
        
        for (int i = 0; i < levelConfig.buildingCount; i++) {
            placeBuilding();
        }
        
        // На уровнях >= 2 добавляем дополнительные здания
        if (levelNumber >= 2) {
            placeBuilding();
        }
    } catch (const LevelGenerationException& e) {
        std::string message = "Ошибка генерации лесного уровня: ";
        message += e.what();
        throw LevelGenerationException(message);
    }
}

// Генерация замкового уровня (средний)
void LevelController::generateCastleLevel() {
    const LevelConfig& levelConfig = config.getLevelConfig(LevelType::CASTLE);
    
    try {
        for (int i = 0; i < levelConfig.enemyCount; i++) {
            placeEnemy();
        }
        
        for (int i = 0; i < levelConfig.buildingCount; i++) {
            placeBuilding();
        }
        
        for (int i = 0; i < levelConfig.towerCount; i++) {
            placeTower();
        }
        
        // На уровнях >= 3 добавляем дополнительные здания
        if (levelNumber >= 3) {
            placeBuilding();
        }
        
        // На уровнях >= 4 добавляем дополнительные башни
        if (levelNumber >= 4) {
            placeTower();
        }
    } catch (const LevelGenerationException& e) {
        std::string message = "Ошибка генерации замкового уровня: ";
        message += e.what();
        throw LevelGenerationException(message);
    }
}

// Генерация подземного уровня (сложный)
void LevelController::generateDungeonLevel() {
    const LevelConfig& levelConfig = config.getLevelConfig(LevelType::DUNGEON);
    
    try {
        for (int i = 0; i < levelConfig.enemyCount; i++) {
            placeEnemy();
        }
        
        for (int i = 0; i < levelConfig.buildingCount; i++) {
            placeBuilding();
        }
        
        for (int i = 0; i < levelConfig.towerCount; i++) {
            placeTower();
        }
        
        // На уровнях >= 3 добавляем дополнительные здания
        if (levelNumber >= 3) {
            placeBuilding();
        }
        
        // На уровнях >= 4 добавляем дополнительные башни
        if (levelNumber >= 4) {
            placeTower();
        }
    } catch (const LevelGenerationException& e) {
        std::string message = "Ошибка генерации подземного уровня: ";
        message += e.what();
        throw LevelGenerationException(message);
    }
}

// Размещение игрока на поле
void LevelController::placePlayer() {
    try {
        auto pos = findValidPosition({-1, -1}, 0);
        player.setPosition(pos);
        
        if (!field.placeObjectAtPosition(pos)) {
            std::string posStr = "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ")";
            throw LevelGenerationException("Не удалось разместить игрока на позиции " + posStr);
        }
    } catch (const FieldException& e) {
        throw LevelGenerationException("Ошибка размещения игрока: " + std::string(e.what()));
    }
}

// Размещение врага на поле
void LevelController::placeEnemy() {
    if (enemies.size() >= maxEnemies) return;
    
    try {
        auto pos = findValidPosition(player.getPosition(), 3);
        enemies.emplace_back(pos, config.getEnemyConfig());
        
        if (!field.placeObjectAtPosition(pos)) {
            std::string posStr = "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ")";
            throw LevelGenerationException("Не удалось разместить врага на позиции " + posStr);
        }
    } catch (const FieldException& e) {
        throw LevelGenerationException("Ошибка размещения врага: " + std::string(e.what()));
    }
}

// Размещение здания на поле
void LevelController::placeBuilding() {
    try {
        auto pos = findValidPosition(player.getPosition(), 5);
        buildings.emplace_back(pos, config.getBuildingConfig());
        
        if (!field.placeObjectAtPosition(pos)) {
            std::string posStr = "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ")";
            throw LevelGenerationException("Не удалось разместить здание на позиции " + posStr);
        }
    } catch (const FieldException& e) {
        throw LevelGenerationException("Ошибка размещения здания: " + std::string(e.what()));
    }
}

// Размещение башни на поле
void LevelController::placeTower() {
    try {
        auto pos = findValidPosition(player.getPosition(), 7);
        towers.emplace_back(pos, config.getTowerConfig());
        
        if (!field.placeObjectAtPosition(pos)) {
            std::string posStr = "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ")";
            throw LevelGenerationException("Не удалось разместить башню на позиции " + posStr);
        }
    } catch (const FieldException& e) {
        throw LevelGenerationException("Ошибка размещения башни: " + std::string(e.what()));
    }
}

// Выдача стартового заклинания игроку
void LevelController::giveStartingSpell() {
    try {
        auto& hand = player.getHand();
        auto spell = SpellFactory::createRandomSpell();
        
        if (spell && hand.hasSpace()) {
            hand.addSpell(spell);
        }
    } catch (const SpellException& e) {
        throw LevelGenerationException("Ошибка выдачи стартового заклинания: " + std::string(e.what()));
    }
}

// Поиск валидной позиции для размещения объектов
Position LevelController::findValidPosition(const Position& avoidPos, int minDistance) {
    const int maxAttempts = 100;
    
    for (int attempt = 0; attempt < maxAttempts; attempt++) {
        int x = std::rand() % field.getWidth();
        int y = std::rand() % field.getHeight();
        Position pos(x, y);
        
        if (field.canMoveToPosition(pos)) {
            if (avoidPos.first == -1 || PositionUtils::calculateDistance(pos, avoidPos) >= minDistance) {
                return pos;
            }
        }
    }
    
    // Исключение вместо возврата некорректной позиции
    throw LevelGenerationException("Не удалось найти валидную позицию после " + 
                                 std::to_string(maxAttempts) + " попыток");
}

// Проверка завершения уровня (все враги уничтожены)
bool LevelController::isCompleted() const {
    bool enemiesAlive = std::any_of(enemies.begin(), enemies.end(), [](const Enemy& e) { return e.isAlive(); });
    bool buildingsAlive = std::any_of(buildings.begin(), buildings.end(), [](const EnemyBuilding& b) { return b.isAlive(); });
    bool towersAlive = std::any_of(towers.begin(), towers.end(), [](const EnemyTower& t) { return t.isAlive(); });
    
    return !enemiesAlive && !buildingsAlive && !towersAlive;
}

// Проверка проигрыша (игрок мертв)
bool LevelController::isFailed() const {
    return !player.isAlive();
}

// Очистка всех объектов уровня
void LevelController::cleanup() {
    enemies.clear();
    buildings.clear();
    towers.clear();
}

// Обработка ходов врагов
void LevelController::processEnemyTurns() {
    try {
        cleanupDeadEntities(enemies);
        aiSystem.processEnemiesTurn(enemies, player, field);
    } catch (const AIException& e) {
        throw AIException("Ошибка обработки ходов врагов: " + std::string(e.what()));
    }
}

// Обработка ходов зданий
void LevelController::processBuildingTurns() {
    try {
        cleanupDeadEntities(buildings);
        for (auto& building : buildings) {
            building.update();
            if (building.canSpawnEnemy() && enemies.size() < maxEnemies) {
                aiSystem.spawnEnemyFromBuilding(building, enemies, field);
            }
        }
    } catch (const AIException& e) {
        throw AIException("Ошибка обработки ходов зданий: " + std::string(e.what()));
    }
}

// Обработка ходов башен
void LevelController::processTowerTurns() {
    try {
        cleanupDeadEntities(towers);
        for (auto& tower : towers) {
            tower.update();
            tower.tryCastSpell(player, field);
        }
    } catch (const AIException& e) {
        throw AIException("Ошибка обработки ходов башен: " + std::string(e.what()));
    }
}
