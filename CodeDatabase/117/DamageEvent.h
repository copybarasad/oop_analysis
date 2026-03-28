#ifndef DAMAGEEVENT_H
#define DAMAGEEVENT_H

#include "Event.h"

class DamageEvent : public Event {
private:
    std::string attackerName;
    std::string targetName;
    int damage;

public:
    DamageEvent(const std::string& attackerName, const std::string& targetName, int damage);
    std::string toString() const override;
};

#endif