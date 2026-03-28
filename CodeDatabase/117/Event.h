#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <ctime>
#include "../Enums.h"

class Event {
private:
    time_t eventTime;

public:
    Event();
    time_t getTime() const;
    virtual std::string toString() const = 0;
};

#endif