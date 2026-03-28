#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "Position.h"
#include <string>
#include <chrono>

enum class EventType {
    PLAYER_MOVED,
    PLAYER_ATTACKED,
    PLAYER_DAMAGED,
    ENEMY_KILLED,
    TOWER_DESTROYED,
    SPELL_USED,
    SPELL_PURCHASED,
    LEVEL_UP,
    GAME_SAVED,
    GAME_LOADED,
    COMMAND_EXECUTED,
    TRIGGER_TRAP,
    ALLY_SUMMONED,
    BUILDING_SPAWNED,
    GAME_STARTED,
    GAME_ENDED
};

struct GameEvent {
    EventType type;
    std::chrono::system_clock::time_point timestamp;
    std::string description;
    
    GameEvent(EventType t, const std::string& desc = "")
        : type(t), timestamp(std::chrono::system_clock::now()), description(desc) {}
    
    virtual ~GameEvent() = default;
    
    virtual std::string toString() const {
        return "[" + std::to_string(static_cast<int>(type)) + "] " + description;
    }
};

struct PlayerMovedEvent : public GameEvent {
    Position from;
    Position to;
    
    PlayerMovedEvent(const Position& f, const Position& t)
        : GameEvent(EventType::PLAYER_MOVED), from(f), to(t) {
        description = "Player moved from (" + std::to_string(from.x) + "," + std::to_string(from.y) +
                     ") to (" + std::to_string(to.x) + "," + std::to_string(to.y) + ")";
    }
};

struct PlayerAttackedEvent : public GameEvent {
    std::string target;
    int damage;
    
    PlayerAttackedEvent(const std::string& targ, int dmg)
        : GameEvent(EventType::PLAYER_ATTACKED), target(targ), damage(dmg) {
        description = "Player attacked " + target + " for " + std::to_string(damage) + " damage";
    }
};

struct PlayerDamagedEvent : public GameEvent {
    int damage;
    std::string source;
    
    PlayerDamagedEvent(int dmg, const std::string& src)
        : GameEvent(EventType::PLAYER_DAMAGED), damage(dmg), source(src) {
        description = "Player took " + std::to_string(damage) + " damage from " + source;
    }
};

struct EnemyKilledEvent : public GameEvent {
    Position position;
    int coinsReward;
    
    EnemyKilledEvent(const Position& pos, int coins)
        : GameEvent(EventType::ENEMY_KILLED), position(pos), coinsReward(coins) {
        description = "Enemy killed at (" + std::to_string(position.x) + "," + std::to_string(position.y) +
                     "), reward: " + std::to_string(coinsReward) + " coins";
    }
};

struct TowerDestroyedEvent : public GameEvent {
    Position position;
    
    TowerDestroyedEvent(const Position& pos)
        : GameEvent(EventType::TOWER_DESTROYED), position(pos) {
        description = "Tower destroyed at (" + std::to_string(position.x) + "," + std::to_string(position.y) + ")";
    }
};

struct SpellUsedEvent : public GameEvent {
    std::string spellName;
    Position target;
    
    SpellUsedEvent(const std::string& name, const Position& targ)
        : GameEvent(EventType::SPELL_USED), spellName(name), target(targ) {
        description = "Spell '" + spellName + "' used at (" + std::to_string(target.x) + 
                     "," + std::to_string(target.y) + ")";
    }
};

struct CommandExecutedEvent : public GameEvent {
    std::string command;
    
    CommandExecutedEvent(const std::string& cmd)
        : GameEvent(EventType::COMMAND_EXECUTED), command(cmd) {
        description = "Command executed: " + command;
    }
};

#endif