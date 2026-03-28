#pragma once
#include <vector>
#include <string>
#include <functional>

enum class EventType {
    PLAYER_MOVE,
    PLAYER_DAMAGE,
    PLAYER_DEATH,

    ENEMY_MOVE,
    ENEMY_ATTACK,
    ENEMY_DAMAGE,
    ENEMY_DEATH,

    SPELL_CAST,
    SPELL_HIT,
    SPELL_MISS,

    TRAP_TRIGGER,
    TOWER_DESTROYED
};


struct Event {
    EventType type;
    std::string source;
    std::string target;
    int x = -1;
    int y = -1;
    int value = 0;
};

class EventDispatcher {
public:
    using Listener = std::function<void(const Event&)>;

    void subscribe(const Listener& listener) {
        listeners.push_back(listener);
    }

    void notify(const Event& event) {
        for (const auto& l : listeners) {
            l(event);
        }
    }

private:
    std::vector<Listener> listeners;
};
