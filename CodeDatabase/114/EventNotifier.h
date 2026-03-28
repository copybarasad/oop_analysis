#ifndef EVENTNOTIFIER_H
#define EVENTNOTIFIER_H

#include "IObserver.h"
#include "IEventSubject.h"
#include "GameEvent.h"
#include <vector>
#include <memory>

class EventNotifier : public IEventSubject {
private:
    std::vector<IObserver*> observers;

public:
    void subscribe(IObserver* observer) override;
    void unsubscribe(IObserver* observer) override;
    void notify(const GameEvent& event);
    
    static EventNotifier& getInstance();
    
private:
    EventNotifier() = default;
    EventNotifier(const EventNotifier&) = delete;
    EventNotifier& operator=(const EventNotifier&) = delete;
};

#endif

