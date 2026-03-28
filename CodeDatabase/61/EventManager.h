#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "GameEvent.h"
#include <vector>
#include <functional>
#include <memory>

// Интерфейс для подписчиков на события
class IEventSubscriber {
public:
    virtual ~IEventSubscriber() = default;
    virtual void onEvent(const std::shared_ptr<GameEvent>& event) = 0;
};

// Менеджер событий (паттерн Observer)
class EventManager {
private:
    std::vector<IEventSubscriber*> subscribers;
    
    EventManager() = default;
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
    
public:
    // Singleton
    static EventManager& getInstance();
    
    // Подписка/отписка на события
    void subscribe(IEventSubscriber* subscriber);
    void unsubscribe(IEventSubscriber* subscriber);
    
    // Публикация события
    void publish(const std::shared_ptr<GameEvent>& event);
    
    // Очистка всех подписчиков
    void clear();
};

#endif // EVENT_MANAGER_H

