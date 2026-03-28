#include "event_manager.h"
#include <algorithm>

EventManager* EventManager::instance = nullptr;

EventManager& EventManager::get_instance()
{
    if (instance == nullptr)
    {
        instance = new EventManager();
    }
    return *instance;
}

void EventManager::add_listener(IEventListener* listener)
{
    listeners.push_back(listener);
}

void EventManager::remove_listener(IEventListener* listener)
{
    listeners.erase(
        std::remove(listeners.begin(), listeners.end(), listener),
        listeners.end()
    );
}

void EventManager::notify(const GameEvent& event)
{
    for (auto* listener : listeners)
    {
        listener->on_event(event);
    }
}

void EventManager::clear_listeners()
{
    listeners.clear();
}