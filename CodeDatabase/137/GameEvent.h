#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <string>
#include <sstream>

enum class EventType {
    GAME_START,
    GAME_END,
    LEVEL_START,
    LEVEL_COMPLETE,
    PLAYER_MOVE,
    PLAYER_ATTACK,
    PLAYER_DAMAGE,
    PLAYER_HEAL,
    PLAYER_DEATH,
    PLAYER_LEVEL_UP,
    ENEMY_SPAWN,
    ENEMY_MOVE,
    ENEMY_ATTACK,
    ENEMY_DEATH,
    ALLY_SPAWN,
    ALLY_MOVE,
    ALLY_ATTACK,
    ALLY_DEATH,
    SPELL_CAST,
    SPELL_ACQUIRED,
    TOWER_ATTACK,
    BUILDING_SPAWN,
    TRAP_TRIGGERED,
    SAVE_GAME,
    LOAD_GAME,
    UPGRADE_SELECTED
};

class GameEvent {
public:
    GameEvent(EventType type, const std::string& description);

    EventType GetType() const;
    const std::string& GetDescription() const;
    std::string ToString() const;

private:
    static std::string EventTypeToString(EventType type);

    EventType type_;
    std::string description_;
};

class GameEventFactory {
public:
    static GameEvent PlayerMove(int from_x, int from_y, int to_x, int to_y);
    static GameEvent PlayerAttack(int target_x, int target_y, int damage);
    static GameEvent PlayerDamage(int damage, int source_damage);
    static GameEvent EnemySpawn(int x, int y, int health);
    static GameEvent EnemyDeath(int x, int y, int score);
    static GameEvent SpellCast(const std::string& spell_name, int target_x, int target_y);
    static GameEvent SpellAcquired(const std::string& spell_name);
    static GameEvent LevelComplete(int level, int score);
    static GameEvent TrapTriggered(int x, int y, int damage);
    static GameEvent AllySpawn(int x, int y);
    static GameEvent SaveGame(const std::string& filename);
    static GameEvent LoadGame(const std::string& filename);
    static GameEvent UpgradeSelected(const std::string& upgrade_name);
};

#endif // GAME_EVENT_H
