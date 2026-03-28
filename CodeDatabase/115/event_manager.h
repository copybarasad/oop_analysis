#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "i_observer.h"
#include <vector>
#include <string>

class EventManager {
private:
    static std::vector<IObserver*> observers;

public:
    static void addObserver(IObserver* observer);
    static void removeObserver(IObserver* observer);

    static void trigger(EventType type, const std::string& msg, int val1 = 0, int val2 = 0);
};

#endif