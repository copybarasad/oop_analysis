#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <vector>
#include "game_event.h"
#include "event_listener.h"

class EventManager
{
private:
    std::vector<IEventListener*> listeners;
    
    static EventManager* instance;
    EventManager() = default;

public:
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
    
    static EventManager& get_instance();
    
    void add_listener(IEventListener* listener);
    void remove_listener(IEventListener* listener);
    void notify(const GameEvent& event);
    
    void clear_listeners();
};

#endif