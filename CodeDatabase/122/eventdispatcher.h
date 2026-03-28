#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <typeindex>
#include "events.h"
#include "qtrenderer.h"

class EventDispatcher
{
public:
    static EventDispatcher& instance() {
        static EventDispatcher instance;
        return instance;
    }

    template <typename T>
    void subscribe(const std::function<void(const T&)>& listener)
    {
        auto& vec = listeners[typeid(T)];
        vec.push_back([listener](const Event& e)
                      { listener(static_cast<const T&>(e)); });
    }

    void notify(const Event& event) const
    {
        auto it = listeners.find(typeid(event));
        if (it != listeners.end())
        {
            for (auto& listener : it->second)
                listener(event);
        }
    }

    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;
    EventDispatcher(EventDispatcher&&) = delete;
    EventDispatcher& operator=(EventDispatcher&&) = delete;

private:
    EventDispatcher() = default;

    std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> listeners;
};

#endif // EVENTDISPATCHER_H
