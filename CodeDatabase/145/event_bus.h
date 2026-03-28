#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include "game_event.h"
#include "event_listener.h"
#include <vector>
#include <memory>

class EventBus {
private:
    static EventBus* instance;
    std::vector<std::shared_ptr<EventListener>> listeners;
    EventBus() = default;

public:
    static EventBus& getInstance();
    void subscribe(std::shared_ptr<EventListener> listener);
    void unsubscribe(std::shared_ptr<EventListener> listener);
    void publish(const std::shared_ptr<GameEvent>& event);
    void clearListeners();
};

#endif