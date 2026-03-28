#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H


#include <memory>
#include <utility>
#include <vector>
#include <cstdlib>
#include "Enemy.h"
#include "Ally.h"
#include "EnemyTower.h"
#include "GameField.h"
#include "GameTypes.h"


/**
 * @file EntityFactory.h
 * @brief Фабрика для создания игровых сущностей
 * 
 * Инкапсулирует логику создания врагов, союзников и башен
 * с поиском подходящих позиций на поле.
 */
class EntityFactory {
public:
    /**
     * @brief Создаёт врага в случайной свободной позиции
     * @param field Указатель на игровое поле
     * @param healthBonus Бонус к здоровью
     * @param damageBonus Бонус к урону
     * @return unique_ptr на созданного врага, nullptr если не удалось найти позицию
     */
    static std::unique_ptr<Enemy> createEnemy(
        const GameField* field,
        int healthBonus = 0,
        int damageBonus = 0
    ) {
        auto pos = findRandomFreePosition(field);
        if (pos.first == -1) return nullptr;
        
        auto enemy = std::make_unique<Enemy>(pos.first, pos.second);
        enemy->setHealth(ENEMY_START_HEALTH + healthBonus);
        enemy->setDamage(ENEMY_START_DAMAGE + damageBonus);
        
        return enemy;
    }
    
    /**
     * @brief Создаёт башню в случайной свободной позиции
     * @param field Указатель на игровое поле
     * @param healthBonus Бонус к здоровью
     * @param damageBonus Бонус к урону (ИСПРАВЛЕНО)
     * @return unique_ptr на созданную башню, nullptr если не удалось найти позицию
     */
    static std::unique_ptr<EnemyTower> createTower(
        const GameField* field,
        int healthBonus = 0,
        int damageBonus = 0  // ← ДОБАВЛЕН ПАРАМЕТР
    ) {
        auto pos = findRandomFreePosition(field);
        if (pos.first == -1) return nullptr;
        
        return std::make_unique<EnemyTower>(
            pos.first, pos.second,
            TOWER_START_HEALTH + healthBonus,
            TOWER_ATTACK_DAMAGE + damageBonus,  // ← ПРИМЕНЯЕМ БОНУС
            TOWER_ATTACK_RANGE,
            2
        );
    }
    
    /**
     * @brief Создаёт союзника рядом с указанной позицией
     * @param field Указатель на игровое поле
     * @param centerX Координата X центра
     * @param centerY Координата Y центра
     * @return unique_ptr на созданного союзника, nullptr если не удалось найти позицию
     */
    static std::unique_ptr<Ally> createAlly(
        const GameField* field,
        int centerX,
        int centerY
    ) {
        auto pos = findFreeAdjacentPosition(field, centerX, centerY);
        if (pos.first == -1) return nullptr;
        
        return std::make_unique<Ally>(pos.first, pos.second);
    }
    
private:
    /**
     * @brief Находит случайную свободную позицию на поле
     * @param field Указатель на игровое поле
     * @return Пара координат (x, y) или (-1, -1) если не нашли за 100 попыток
     */
    static std::pair<int, int> findRandomFreePosition(const GameField* field) {
        int width = field->getWidth();
        int height = field->getHeight();
        
        // Пытаемся 100 раз найти свободную клетку
        for (int attempt = 0; attempt < 100; ++attempt) {
            int x = rand() % width;
            int y = rand() % height;
            
            if (field->isCellPassable(x, y)) {
                return {x, y};
            }
        }
        
        // Не нашли - возвращаем невалидную позицию
        return {-1, -1};
    }
    
    /**
     * @brief Находит свободную позицию рядом с центром
     * @param field Указатель на игровое поле
     * @param centerX Координата X центра
     * @param centerY Координата Y центра
     * @return Пара координат (x, y) или (-1, -1) если не нашли
     */
    static std::pair<int, int> findFreeAdjacentPosition(
        const GameField* field,
        int centerX,
        int centerY
    ) {
        // Проверяем 8 направлений вокруг центра
        std::vector<std::pair<int, int>> offsets = {
            {-1, -1}, {0, -1}, {1, -1},
            {-1,  0},          {1,  0},
            {-1,  1}, {0,  1}, {1,  1}
        };
        
        for (auto [dx, dy] : offsets) {
            int x = centerX + dx;
            int y = centerY + dy;
            
            if (x >= 0 && x < field->getWidth() && 
                y >= 0 && y < field->getHeight() &&
                field->isCellPassable(x, y)) {
                return {x, y};
            }
        }
        
        // Если рядом не нашли - ищем случайную
        return findRandomFreePosition(field);
    }
};


#endif // ENTITYFACTORY_H


