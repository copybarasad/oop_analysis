#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

enum class EventType {
    PLAYER_MOVE,
    PLAYER_ATTACK,
    PLAYER_CAST_SPELL,
    PLAYER_DAMAGE_TAKEN,
    ENEMY_DESTROYED,
    BASE_DESTROYED,
    TOWER_DESTROYED,
    ALLY_SUMMONED,
    LEVEL_COMPLETE,
    GAME_SAVED,
    GAME_LOADED,
    TRAP_TRIGGERED,
    SPELL_OBTAINED,
    PLAYER_HEALED,
    UPGRADE_APPLIED
};

struct GameEvent {
    EventType type;
    std::string actor;
    std::string target;
    int value;
    int x, y;
    std::string details;
    
    std::string toString() const {
        std::stringstream ss;
        ss << "[";
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        ss << std::put_time(&tm, "%H:%M:%S") << "] ";
        
        switch(type) {
            case EventType::PLAYER_MOVE: ss << "PLAYER_MOVE"; break;
            case EventType::PLAYER_ATTACK: ss << "PLAYER_ATTACK"; break;
            case EventType::PLAYER_CAST_SPELL: ss << "SPELL_CAST"; break;
            case EventType::PLAYER_DAMAGE_TAKEN: ss << "DAMAGE_TAKEN"; break;
            case EventType::ENEMY_DESTROYED: ss << "ENEMY_DESTROYED"; break;
            case EventType::BASE_DESTROYED: ss << "BASE_DESTROYED"; break;
            case EventType::TOWER_DESTROYED: ss << "TOWER_DESTROYED"; break;
            case EventType::ALLY_SUMMONED: ss << "ALLY_SUMMONED"; break;
            case EventType::LEVEL_COMPLETE: ss << "LEVEL_COMPLETE"; break;
            case EventType::GAME_SAVED: ss << "GAME_SAVED"; break;
            case EventType::GAME_LOADED: ss << "GAME_LOADED"; break;
            case EventType::TRAP_TRIGGERED: ss << "TRAP_TRIGGERED"; break;
            case EventType::SPELL_OBTAINED: ss << "SPELL_OBTAINED"; break;
            case EventType::PLAYER_HEALED: ss << "PLAYER_HEALED"; break;
            case EventType::UPGRADE_APPLIED: ss << "UPGRADE_APPLIED"; break;
        }
        
        if (!actor.empty()) ss << " Actor: " << actor;
        if (!target.empty()) ss << " Target: " << target;
        if (value != 0) ss << " Value: " << value;
        if (x != -1 && y != -1) ss << " Position: (" << x << "," << y << ")";
        if (!details.empty()) ss << " Details: " << details;
        
        return ss.str();
    }
};

class EventObserver {
public:
    virtual ~EventObserver() = default;
    virtual void onEvent(const GameEvent& event) = 0;
};

class EventSystem {
private:
    static EventSystem* instance;
    std::vector<std::shared_ptr<EventObserver>> observers;
    
    EventSystem() = default;
    
public:
    static EventSystem& getInstance() {
        static EventSystem instance;
        return instance;
    }
    
    void addObserver(std::shared_ptr<EventObserver> observer) {
        observers.push_back(observer);
    }
    
    void removeObserver(std::shared_ptr<EventObserver> observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
    
    void notify(const GameEvent& event) {
        for (const auto& observer : observers) {
            observer->onEvent(event);
        }
    }
    
    EventSystem(const EventSystem&) = delete;
    EventSystem& operator=(const EventSystem&) = delete;
};