#ifndef DEATHEVENT_H
#define DEATHEVENT_H

#include "Event.h"

class DeathEvent : public Event {
private:
    std::string characterName;

public:
    DeathEvent(const std::string& characterName);
    std::string toString() const override;
};

#endif