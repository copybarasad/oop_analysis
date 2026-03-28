#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <string>
#include <utility>
#include <chrono>

class GameEvent {
public:
    enum class Type {
        PLAYER_MOVED,
        PLAYER_ATTACKED,
        PLAYER_TOOK_DAMAGE,
        PLAYER_DIED,
        ENEMY_SPAWNED,
        ENEMY_MOVED,
        ENEMY_ATTACKED,
        ENEMY_TOOK_DAMAGE,
        ENEMY_DIED,
        ALLY_SPAWNED,
        ALLY_MOVED,
        ALLY_ATTACKED,
        ALLY_TOOK_DAMAGE,
        ALLY_DIED,
        SPELL_CAST,
        SPELL_ACQUIRED,
        GAME_STARTED,
        LEVEL_COMPLETED,
        GAME_OVER
    };

    Type type;
    std::string description;
    std::chrono::system_clock::time_point timestamp;

    GameEvent(Type t, const std::string& desc)
        : type(t), description(desc), timestamp(std::chrono::system_clock::now()) {}

    virtual ~GameEvent() = default;

    static std::string typeToString(Type t);
};

class MoveEvent : public GameEvent {
public:
    std::string entityName;
    std::pair<int, int> fromPos;
    std::pair<int, int> toPos;

    MoveEvent(const std::string& entity, std::pair<int, int> from, std::pair<int, int> to)
        : GameEvent(Type::PLAYER_MOVED, entity + " moved from (" + std::to_string(from.first) + "," +
                   std::to_string(from.second) + ") to (" + std::to_string(to.first) + "," +
                   std::to_string(to.second) + ")"),
          entityName(entity), fromPos(from), toPos(to) {}
};

class AttackEvent : public GameEvent {
public:
    std::string attacker;
    std::string defender;
    int damage;

    AttackEvent(const std::string& att, const std::string& def, int dmg)
        : GameEvent(Type::PLAYER_ATTACKED, att + " dealt " + std::to_string(dmg) + 
                   " damage to " + def),
          attacker(att), defender(def), damage(dmg) {}
};

class DamageEvent : public GameEvent {
public:
    std::string entity;
    int damage;
    int remainingHealth;

    DamageEvent(const std::string& ent, int dmg, int health)
        : GameEvent(Type::PLAYER_TOOK_DAMAGE, ent + " took " + std::to_string(dmg) + 
                   " damage. Health: " + std::to_string(health)),
          entity(ent), damage(dmg), remainingHealth(health) {}
};

class DeathEvent : public GameEvent {
public:
    std::string entity;
    int reward;

    DeathEvent(const std::string& ent, int rew = 0)
        : GameEvent(Type::PLAYER_DIED, ent + " died" + 
                   (rew > 0 ? " (+" + std::to_string(rew) + " points)" : "")),
          entity(ent), reward(rew) {}
};

class SpellCastEvent : public GameEvent {
public:
    std::string caster;
    std::string spellName;
    std::pair<int, int> targetPos;

    SpellCastEvent(const std::string& cast, const std::string& spell, std::pair<int, int> target)
        : GameEvent(Type::SPELL_CAST, cast + " cast " + spell + " at (" + 
                   std::to_string(target.first) + "," + std::to_string(target.second) + ")"),
          caster(cast), spellName(spell), targetPos(target) {}
};

class SpellAcquiredEvent : public GameEvent {
public:
    std::string entity;
    std::string spellName;

    SpellAcquiredEvent(const std::string& ent, const std::string& spell)
        : GameEvent(Type::SPELL_ACQUIRED, ent + " acquired " + spell),
          entity(ent), spellName(spell) {}
};

class GameStartedEvent : public GameEvent {
public:
    int level;

    GameStartedEvent(int lvl)
        : GameEvent(Type::GAME_STARTED, "Game started at level " + std::to_string(lvl)),
          level(lvl) {}
};

class LevelCompletedEvent : public GameEvent {
public:
    int level;
    int score;

    LevelCompletedEvent(int lvl, int pts)
        : GameEvent(Type::LEVEL_COMPLETED, "Level " + std::to_string(lvl) + 
                   " completed with score " + std::to_string(pts)),
          level(lvl), score(pts) {}
};

class GameOverEvent : public GameEvent {
public:
    bool won;
    int finalScore;

    GameOverEvent(bool victory, int score)
        : GameEvent(Type::GAME_OVER, victory ? "Game won!" : "Game over!"),
          won(victory), finalScore(score) {}
};

#endif