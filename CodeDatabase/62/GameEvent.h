#pragma once
#include <string>

// Типы игровых событий
enum class GameEventType {
    PlayerMoved,
    PlayerAttacked,
    DamageDealt,
    EntityDied,
    SpellAcquired,
    SpellUsed,
    AttackModeChanged,
    GameSaved,
    GameLoaded,
    ShopItemPurchased,
    ScoreChanged
};

// Базовая структура игрового события
struct GameEvent {
    GameEventType type;
    
    // Общие поля
    std::string entityName;  // Имя сущности (игрок, враг, и т.д.)
    int x = -1;               // Координата X
    int y = -1;               // Координата Y
    int value = 0;            // Значение (урон, очки, и т.д.)
    std::string description;  // Описание события
    
    GameEvent(GameEventType t) : type(t) {}
};

// Специализированные события
struct DamageEvent : public GameEvent {
    std::string targetName;  // Кто получил урон
    int damage;              // Количество урона
    
    DamageEvent(const std::string& attacker, const std::string& target, int dmg, int targetX, int targetY)
        : GameEvent(GameEventType::DamageDealt), targetName(target), damage(dmg) {
        entityName = attacker;
        x = targetX;
        y = targetY;
        value = dmg;
        description = attacker + " dealt " + std::to_string(dmg) + " damage to " + target + " at (" + 
                     std::to_string(targetX) + ", " + std::to_string(targetY) + ")";
    }
};

struct MovementEvent : public GameEvent {
    int fromX, fromY;
    int toX, toY;
    
    MovementEvent(const std::string& entity, int fx, int fy, int tx, int ty)
        : GameEvent(GameEventType::PlayerMoved), fromX(fx), fromY(fy), toX(tx), toY(ty) {
        entityName = entity;
        x = tx;
        y = ty;
        description = entity + " moved from (" + std::to_string(fx) + ", " + std::to_string(fy) + 
                     ") to (" + std::to_string(tx) + ", " + std::to_string(ty) + ")";
    }
};

struct SpellEvent : public GameEvent {
    std::string spellName;
    
    SpellEvent(GameEventType t, const std::string& spell, int spellX = -1, int spellY = -1)
        : GameEvent(t), spellName(spell) {
        x = spellX;
        y = spellY;
        if (t == GameEventType::SpellAcquired) {
            description = "Spell acquired: " + spell;
        } else if (t == GameEventType::SpellUsed) {
            description = "Spell used: " + spell + " at (" + std::to_string(spellX) + ", " + 
                         std::to_string(spellY) + ")";
        }
    }
};

struct EntityDeathEvent : public GameEvent {
    std::string entityType;
    
    EntityDeathEvent(const std::string& entity, const std::string& type, int ex, int ey)
        : GameEvent(GameEventType::EntityDied), entityType(type) {
        entityName = entity;
        x = ex;
        y = ey;
        description = type + " " + entity + " died at (" + std::to_string(ex) + ", " + std::to_string(ey) + ")";
    }
};

