#ifndef TRAPTRIGGEREVENT_H
#define TRAPTRIGGEREVENT_H

#include "Event.h"

class TrapTriggerEvent : public Event {
private:
    std::string target;
    int damage;
    std::pair <int, int> position;

public:
    TrapTriggerEvent(const std::string& target, int damage, std::pair<int, int> position);
    std::string toString() const override;
};

#endif