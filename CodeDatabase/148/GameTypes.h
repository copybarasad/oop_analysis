#ifndef GAMETYPES_H
#define GAMETYPES_H

/**
 * @file GameTypes.h
 * @brief Определение перечислений и констант для игры
 */

/**
 * @enum AttackMode
 * @brief Режимы атаки игрока
 */
enum class AttackMode {
    MELEE,  ///< Ближний бой - увеличенный урон
    RANGED  ///< Дальний бой - стандартный урон
};

/**
 * @enum CellType  
 * @brief Типы клеток игрового поля
 */
enum class CellType {
    EMPTY,   ///< Пустая проходимая клетка
    BLOCKED, ///< Непроходимая клетка (стена)
    SLOW     ///< Замедляющая клетка (для расширения)
};

/**
 * @enum Direction
 * @brief Направления движения
 */
enum class Direction {
    UP,    ///< Движение вверх
    DOWN,  ///< Движение вниз  
    LEFT,  ///< Движение влево
    RIGHT  ///< Движение вправо
};

// Константы для настройки баланса игры
constexpr int DEFAULT_FIELD_SIZE = 15;           ///< Стандартный размер поля
constexpr int MIN_FIELD_SIZE = 10;               ///< Минимальный размер поля
constexpr int MAX_FIELD_SIZE = 25;               ///< Максимальный размер поля
constexpr int PLAYER_START_HEALTH = 100;         ///< Начальное здоровье игрока
constexpr int ENEMY_START_HEALTH = 50;           ///< Начальное здоровье врага
constexpr int PLAYER_START_DAMAGE = 40;          ///< Базовый урон игрока
constexpr int ENEMY_START_DAMAGE = 15;           ///< Базовый урон врага
constexpr int TOWER_START_HEALTH = 80;           ///< Начальное здоровье башни
constexpr int TOWER_ATTACK_RANGE = 4;            ///< Радиус атаки башни
constexpr int TOWER_ATTACK_DAMAGE = 15;          ///< Базовый урон башни 
constexpr int RANGED_ATTACK_RANGE = 3;           ///< Радиус дальнобойной атаки
constexpr float MELEE_DAMAGE_MULTIPLIER = 1.0f;  ///< Множитель урона в MELEE режиме (x1)
constexpr float RANGED_DAMAGE_MULTIPLIER = 0.5f; ///< Множитель урона в RANGED режиме (x0.5)
constexpr int SPELL_COST = 10;  ///< Стоимость одного заклинания


#endif // GAMETYPES_H


