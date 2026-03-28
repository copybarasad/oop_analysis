#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <string>
#include <ctime>

enum class EventType {
    DAMAGE_DEALT,
    PLAYER_MOVED,
    SPELL_OBTAINED,
    SPELL_CAST,
    COMBAT_MODE_SWITCHED,
    ENEMY_SPAWNED,
    ENTITY_DEATH,
    PLAYER_ATTACKED,
    TRAP_TRIGGERED,
    PET_SUMMONED,
    FIELD_RENDER_REQUEST,
    MESSAGE_PRINT
};

class GameEvent {
private:
    EventType type;
    std::string description;
    time_t timestamp;

public:
    GameEvent(EventType eventType, const std::string& eventDescription);
    
    EventType getType() const { return type; }
    const std::string& getDescription() const { return description; }
    time_t getTimestamp() const { return timestamp; }
    
    virtual std::string toString() const;
};

#endif

