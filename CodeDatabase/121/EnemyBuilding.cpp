#include "EnemyBuilding.h"

// Реализация здания-спаунера врагов
// Содержит логику спауна врагов, использует конфигурацию
// для настройки времени перезарядки.

// Конструктор здания - использует конфигурацию для настроек
EnemyBuilding::EnemyBuilding(const Position& pos, const BuildingConfig& config)
    : Entity(config.health, pos, "EnemyBuilding", config.reward),
      maxCooldown(config.spawnCooldown) {
    spawnCooldown = config.spawnCooldown;
    currentCooldown = 0;  // Может спаунить сразу
}

// Получение урона - базовая реализация
void EnemyBuilding::takeDamage(int damageAmount) {
    Entity::takeDamage(damageAmount);
}

// Обновление состояния - уменьшение кд спауна
void EnemyBuilding::update() {
    if (currentCooldown > 0) {
        currentCooldown--;  // Уменьшаем таймер каждый ход
    }
}

// Проверка может ли здание спаунить врага
bool EnemyBuilding::canSpawnEnemy() const {
    return currentCooldown == 0 && isAlive();  // Кд истек и здание живо
}

// Запуск спауна врага - сбрасывает таймер
void EnemyBuilding::spawnEnemy() {
    currentCooldown = spawnCooldown;  // Сбрасываем таймер
}

// Получение доступных позиций вокруг здания для спауна
std::vector<Position> EnemyBuilding::getSpawnPositions(const GameField& field) const {
    std::vector<Position> spawnPositions;
    // Все 4 направления вокруг здания
    std::vector<Position> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    // Проверяем каждую направление на возможность спауна
    for (const auto& direction : directions) {
        Position spawnPos(position.first + direction.first, position.second + direction.second);
        if (field.canMoveToPosition(spawnPos)) {
            spawnPositions.push_back(spawnPos);  // Добавляем если клетка свободна
        }
    }
    return spawnPositions;
}