#ifndef LEVEL_CONTROLLER_H
#define LEVEL_CONTROLLER_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "AISystem.h"
#include "LevelType.h"
#include "GameConfig.h"  
#include <vector>


// Управление игровым уровнем
// Создает и управляет всеми объектами на уровне.
// Использует конфигурацию для настройки уровня и объектов.


class LevelController {
private:
    int levelNumber;                    // Номер уровня
    LevelType type;                     // Тип уровня
    GameConfig config;                  // Конфигурация игры 
    GameField field;                    // Игровое поле
    Player player;                      // Игрок
    std::vector<Enemy> enemies;         // Враги на уровне
    std::vector<EnemyBuilding> buildings; // Здания на уровне
    std::vector<EnemyTower> towers;     // Башни на уровне
    AISystem aiSystem;                  // Система ИИ
    size_t maxEnemies;                  // Максимальное количество врагов

public:
    // Основной конструктор уровня с конфигурацией
    LevelController(int number, LevelType levelType, const GameConfig& gameConfig);
    
    // Конструктор копирования
    LevelController(const LevelController& other);
    
    // Геттеры
    int getLevelNumber() const { return levelNumber; }
    LevelType getLevelType() const { return type; }
    size_t getMaxEnemies() const { return maxEnemies; }
    
    const GameField& getField() const { return field; }
    const Player& getPlayer() const { return player; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    const std::vector<EnemyBuilding>& getBuildings() const { return buildings; }
    const std::vector<EnemyTower>& getTowers() const { return towers; }
    
    GameField& getField() { return field; }
    Player& getPlayer() { return player; }
    std::vector<Enemy>& getEnemies() { return enemies; }
    std::vector<EnemyBuilding>& getBuildings() { return buildings; }
    std::vector<EnemyTower>& getTowers() { return towers; }
    
    // Основные методы уровня
    void initialize();
    void cleanup();
    bool isCompleted() const;
    bool isFailed() const;
    
    // Методы обработки ходов
    void processEnemyTurns();
    void processBuildingTurns();
    void processTowerTurns();

private:
    // Методы генерации уровня
    void generateLevel();
    void generateForestLevel();
    void generateCastleLevel();
    void generateDungeonLevel();
    
    // Методы размещения объектов
    void placePlayer();
    void placeEnemy();
    void placeBuilding();
    void placeTower();
    void giveStartingSpell();
    
    // Вспомогательные методы
    Position findValidPosition(const Position& avoidPos, int minDistance);
    
    // Шаблонный метод для очистки мертвых объектов
    template<typename Container>
    void cleanupDeadEntities(Container& container) {
        for (auto it = container.begin(); it != container.end(); ) {
            if (!it->isAlive()) {
                field.removeObjectFromPosition(it->getPosition());
                it = container.erase(it);
            } else {
                ++it;
            }
        }
    }
};

#endif