#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include "Position.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>

enum class GameEventType {
    COMMAND_INPUT,
    
    PLAYER_MOVED,
    PLAYER_ATTACKED,
    PLAYER_DAMAGED,
    PLAYER_MODE_SWITCHED,
    PLAYER_SPELL_CAST,
    PLAYER_SPELL_BOUGHT,
    PLAYER_UPGRADE_CHOSEN,
    
    ENEMY_MOVED,
    ENEMY_ATTACKED,
    ENEMY_DAMAGED,
    ENEMY_KILLED,
    ENEMY_SPAWNED,
    
    BUILDING_DAMAGED,
    BUILDING_DESTROYED,
    BUILDING_SPAWNED_ENEMY,
    
    TOWER_ATTACKED,
    TOWER_DAMAGED,
    TOWER_DESTROYED,
    
    ALLY_MOVED,
    ALLY_ATTACKED,
    ALLY_DAMAGED,
    ALLY_KILLED,
    ALLY_SUMMONED,
    
    TRAP_PLACED,
    TRAP_TRIGGERED,
    
    LEVEL_STARTED,
    LEVEL_COMPLETED,
    GAME_OVER,
    GAME_VICTORY,
    GAME_SAVED,
    GAME_LOADED,
    
    TURN_STARTED,
    TURN_ENDED
};

struct GameEvent {
    GameEventType type;
    Position position;
    Position targetPosition;
    int value1;
    int value2;
    std::string description;
    
    GameEvent(GameEventType t, const std::string& desc = "")
        : type(t), position(0, 0), targetPosition(0, 0), 
          value1(0), value2(0), description(desc) {}
    
    GameEvent(GameEventType t, Position pos, const std::string& desc = "")
        : type(t), position(pos), targetPosition(0, 0), 
          value1(0), value2(0), description(desc) {}
    
    GameEvent(GameEventType t, Position pos, int val, const std::string& desc = "")
        : type(t), position(pos), targetPosition(0, 0), 
          value1(val), value2(0), description(desc) {}
    
    GameEvent(GameEventType t, Position from, Position to, int val, const std::string& desc = "")
        : type(t), position(from), targetPosition(to), 
          value1(val), value2(0), description(desc) {}
};

class IGameEventListener {
public:
    virtual ~IGameEventListener() = default;
    virtual void onEvent(const GameEvent& event) = 0;
};

class GameEventDispatcher {
private:
    std::vector<IGameEventListener*> listeners;
    
    GameEventDispatcher() = default;
    
public:
    static GameEventDispatcher& getInstance();
    
    void addListener(IGameEventListener* listener);
    void removeListener(IGameEventListener* listener);
    void dispatch(const GameEvent& event);
    void clear();
};

#endif
