#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "Entity.h"
#include "GameConfig.h"
#include "GameField.h"
#include <vector>


// Класс здания-спаунера врагов
// Создает новых врагов с заданным интервалом.
// Использует конфигурацию для настройки времени спауна.


class EnemyBuilding : public Entity {
private:
    int spawnCooldown;    // Время между спаунами врагов
    int currentCooldown;  // Текущее время до следующего спауна
    int maxCooldown;      // Максимальное время перезарядки

public:
    // Конструктор с позицией и конфигурацией
    EnemyBuilding(const Position& pos, const BuildingConfig& config);
    
    // Геттеры для отслеживания перезарядки
    int getCurrentCooldown() const { return currentCooldown; }
    int getMaxCooldown() const { return maxCooldown; }
    
    // Методы урона и обновления
    void takeDamage(int damageAmount) override;
    void update() override;
    
    // Методы спауна врагов
    bool canSpawnEnemy() const;
    void spawnEnemy();
    
    // Получение доступных позиций для спауна
    std::vector<Position> getSpawnPositions(const GameField& field) const;
};

#endif