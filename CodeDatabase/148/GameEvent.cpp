#include "GameEvent.h"
#include <sstream>
#include <iomanip>

/**
 * @file GameEvent.cpp
 * @brief Реализация игровых событий
 */

/**
 * @brief Получить строковое представление типа
 */
std::string GameEvent::getTypeString() const {
    switch (type) {
        case EventType::PLAYER_MOVED:        return "PLAYER_MOVED";
        case EventType::PLAYER_DAMAGED:      return "PLAYER_DAMAGED";
        case EventType::PLAYER_HEALED:       return "PLAYER_HEALED";
        case EventType::PLAYER_ATTACK:       return "PLAYER_ATTACK";
        case EventType::ENEMY_SPAWNED:       return "ENEMY_SPAWNED";
        case EventType::ENEMY_DAMAGED:       return "ENEMY_DAMAGED";
        case EventType::ENEMY_DIED:          return "ENEMY_DIED";
        case EventType::ENEMY_ATTACK:        return "ENEMY_ATTACK";
        case EventType::SPELL_CAST:          return "SPELL_CAST";
        case EventType::SPELL_ADDED:         return "SPELL_ADDED";
        case EventType::SPELL_REMOVED:       return "SPELL_REMOVED";
        case EventType::TRAP_ACTIVATED:      return "TRAP_ACTIVATED";
        case EventType::ALLY_SPAWNED:        return "ALLY_SPAWNED";
        case EventType::ALLY_DIED:           return "ALLY_DIED";
        case EventType::TOWER_DESTROYED:     return "TOWER_DESTROYED";
        case EventType::LEVEL_COMPLETED:     return "LEVEL_COMPLETED";
        case EventType::LEVEL_STARTED:       return "LEVEL_STARTED";
        case EventType::GAME_SAVED:          return "GAME_SAVED";
        case EventType::GAME_LOADED:         return "GAME_LOADED";
        case EventType::ATTACK_MODE_SWITCHED: return "ATTACK_MODE_SWITCHED";
        default:                             return "UNKNOWN";
    }
}

/**
 * @brief Получить форматированное время
 */
std::string GameEvent::getTimestampString() const {
    std::tm* tm = std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}


