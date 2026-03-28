#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <vector>
#include <algorithm>


enum class EventType {
    INFO,
    DAMAGE,
    MOVE,
    DEATH,
    ERROR,
    LEVEL_UP
};


class IObserver {
public:
    virtual void onNotify(EventType type, const std::string& message) = 0;
    virtual ~IObserver() = default;
};


class IObservable {
protected:
    std::vector<IObserver*> observers;

public:
    virtual ~IObservable() = default;

    void addObserver(IObserver* obs) {
        if (obs) {
            observers.push_back(obs);
        }
    }

    void removeObserver(IObserver* obs) {
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void notify(EventType type, const std::string& message) const {
        for (auto* obs : observers) {
            obs->onNotify(type, message);
        }
    }
};

#endif