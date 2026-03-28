#pragma once
#include <string>
#include <variant>

enum class GameEventType {
    PLAYER_MOVED,
    PLAYER_ATTACKED,
    PLAYER_DAMAGED,
    PLAYER_HEALED,
    ENEMY_MOVED,
    ENEMY_ATTACKED,
    ENEMY_DAMAGED,
    ENEMY_KILLED,
    BUILDING_DAMAGED,
    BUILDING_DESTROYED,
    SPELL_RECEIVED,
    SPELL_USED,
    TRAP_TRIGGERED,
    LEVEL_STARTED,
    LEVEL_COMPLETED,
    GAME_SAVED,
    GAME_LOADED
};

struct GameEvent {
    GameEventType type;
    std::string description;
    
    int x = 0;
    int y = 0;
    int damage = 0;
    int health = 0;
    std::string entity_name;
    std::string spell_name;
    int level = 0;
};

namespace Events {
    inline GameEvent PlayerMoved(int x, int y) {
        return {GameEventType::PLAYER_MOVED, "Игрок переместился", x, y, 0, 0, "", "", 0};
    }
    
    inline GameEvent PlayerAttacked(const std::string& target, int damage) {
        return {GameEventType::PLAYER_ATTACKED, "Игрок атаковал", 0, 0, damage, 0, target, "", 0};
    }
    
    inline GameEvent PlayerDamaged(int damage, int remaining_health) {
        return {GameEventType::PLAYER_DAMAGED, "Игрок получил урон", 0, 0, damage, remaining_health, "", "", 0};
    }
    
    inline GameEvent EnemyMoved(const std::string& name, int x, int y) {
        return {GameEventType::ENEMY_MOVED, "Враг переместился", x, y, 0, 0, name, "", 0};
    }
    
    inline GameEvent EnemyDamaged(const std::string& name, int damage, int remaining_health) {
        return {GameEventType::ENEMY_DAMAGED, "Враг получил урон", 0, 0, damage, remaining_health, name, "", 0};
    }
    
    inline GameEvent EnemyKilled(const std::string& name) {
        return {GameEventType::ENEMY_KILLED, "Враг убит", 0, 0, 0, 0, name, "", 0};
    }
    
    inline GameEvent BuildingDamaged(int x, int y, int damage, int remaining_health) {
        return {GameEventType::BUILDING_DAMAGED, "Здание повреждено", x, y, damage, remaining_health, "", "", 0};
    }
    
    inline GameEvent BuildingDestroyed(int x, int y) {
        return {GameEventType::BUILDING_DESTROYED, "Здание разрушено", x, y, 0, 0, "", "", 0};
    }
    
    inline GameEvent SpellReceived(const std::string& spell_name) {
        return {GameEventType::SPELL_RECEIVED, "Получено заклинание", 0, 0, 0, 0, "", spell_name, 0};
    }
    
    inline GameEvent SpellUsed(const std::string& spell_name, int x, int y) {
        return {GameEventType::SPELL_USED, "Использовано заклинание", x, y, 0, 0, "", spell_name, 0};
    }
    
    inline GameEvent TrapTriggered(const std::string& victim, int damage) {
        return {GameEventType::TRAP_TRIGGERED, "Ловушка сработала", 0, 0, damage, 0, victim, "", 0};
    }
    
    inline GameEvent LevelStarted(int level) {
        return {GameEventType::LEVEL_STARTED, "Уровень начат", 0, 0, 0, 0, "", "", level};
    }
    
    inline GameEvent LevelCompleted(int level) {
        return {GameEventType::LEVEL_COMPLETED, "Уровень завершён", 0, 0, 0, 0, "", "", level};
    }
    
    inline GameEvent GameSaved(const std::string& filename) {
        return {GameEventType::GAME_SAVED, "Игра сохранена", 0, 0, 0, 0, filename, "", 0};
    }
    
    inline GameEvent GameLoaded(const std::string& filename) {
        return {GameEventType::GAME_LOADED, "Игра загружена", 0, 0, 0, 0, filename, "", 0};
    }
}
