#pragma once
#include <string>
#include <vector>

class GameEvent {
public:
    virtual ~GameEvent() = default;
    virtual std::string toString() const = 0;
    virtual std::string getType() const = 0;
};

class EventListener {
public:
    virtual ~EventListener() = default;
    virtual void onEvent(const GameEvent& event) = 0;
};

class EventSystem {
private:
    std::vector<EventListener*> listeners_;
    
public:
    void subscribe(EventListener* listener);
    void unsubscribe(EventListener* listener);
    void notify(const GameEvent& event) const;
};