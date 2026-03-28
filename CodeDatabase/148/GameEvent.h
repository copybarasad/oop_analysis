#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <string>
#include <ctime>

/**
 * @file GameEvent.h
 * @brief Определение игровых событий
 */

/**
 * @enum EventType
 * @brief Типы игровых событий
 */
enum class EventType {
    PLAYER_MOVED,           ///< Игрок переместился
    PLAYER_DAMAGED,         ///< Игрок получил урон
    PLAYER_HEALED,          ///< Игрок восстановил здоровье
    PLAYER_ATTACK,          ///< Игрок атаковал
    ENEMY_SPAWNED,          ///< Враг появился
    ENEMY_DAMAGED,          ///< Враг получил урон
    ENEMY_DIED,             ///< Враг умер
    ENEMY_ATTACK,           ///< Враг атаковал
    SPELL_CAST,             ///< Заклинание использовано
    SPELL_ADDED,            ///< Заклинание добавлено в руку
    SPELL_REMOVED,          ///< Заклинание удалено из руки
    TRAP_ACTIVATED,         ///< Ловушка активирована
    ALLY_SPAWNED,           ///< Союзник призван
    ALLY_DIED,              ///< Союзник умер
    TOWER_DESTROYED,        ///< Башня уничтожена
    LEVEL_COMPLETED,        ///< Уровень пройден
    LEVEL_STARTED,          ///< Уровень начат
    GAME_SAVED,             ///< Игра сохранена
    GAME_LOADED,            ///< Игра загружена
    ATTACK_MODE_SWITCHED    ///< Режим атаки переключён
};

/**
 * @struct GameEvent
 * @brief Базовая структура игрового события
 */
struct GameEvent {
    EventType type;           ///< Тип события
    std::time_t timestamp;    ///< Время события
    std::string description;  ///< Текстовое описание
    
    /**
     * @brief Конструктор
     */
    GameEvent(EventType t, const std::string& desc) 
        : type(t), timestamp(std::time(nullptr)), description(desc) {}
    
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~GameEvent() = default;
    
    /**
     * @brief Получить строковое представление типа события
     */
    std::string getTypeString() const;
    
    /**
     * @brief Получить форматированное время
     */
    std::string getTimestampString() const;
};

/**
 * @struct DamageEvent
 * @brief Событие нанесения урона
 */
struct DamageEvent : public GameEvent {
    std::string attacker;  ///< Кто атаковал
    std::string target;    ///< Кто получил урон
    int damage;            ///< Количество урона
    int remainingHP;       ///< Оставшееся HP цели
    
    DamageEvent(EventType t, const std::string& atk, const std::string& tgt, int dmg, int hp)
        : GameEvent(t, ""), attacker(atk), target(tgt), damage(dmg), remainingHP(hp) {
        description = attacker + " dealt " + std::to_string(damage) + 
                      " damage to " + target + " (HP: " + std::to_string(remainingHP) + ")";
    }
};

/**
 * @struct MovementEvent
 * @brief Событие движения
 */
struct MovementEvent : public GameEvent {
    std::string entity;  ///< Кто двигался
    int fromX, fromY;    ///< Откуда
    int toX, toY;        ///< Куда
    
    MovementEvent(const std::string& ent, int fx, int fy, int tx, int ty)
        : GameEvent(EventType::PLAYER_MOVED, ""), entity(ent), 
          fromX(fx), fromY(fy), toX(tx), toY(ty) {
        description = entity + " moved from (" + std::to_string(fromX) + "," + std::to_string(fromY) + 
                      ") to (" + std::to_string(toX) + "," + std::to_string(toY) + ")";
    }
};

/**
 * @struct SpellEvent
 * @brief Событие заклинания
 */
struct SpellEvent : public GameEvent {
    std::string spellName;  ///< Название заклинания
    
    SpellEvent(EventType t, const std::string& spell, const std::string& desc)
        : GameEvent(t, desc), spellName(spell) {}
};

/**
 * @struct EntityEvent
 * @brief Событие сущности (спавн/смерть)
 */
struct EntityEvent : public GameEvent {
    std::string entityType;  ///< Тип сущности
    int x, y;                ///< Координаты
    
    EntityEvent(EventType t, const std::string& type, int posX, int posY, const std::string& desc)
        : GameEvent(t, desc), entityType(type), x(posX), y(posY) {}
};

#endif // GAMEEVENT_H


