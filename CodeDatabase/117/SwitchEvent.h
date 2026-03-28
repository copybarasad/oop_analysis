#ifndef SWITCHEVENT_H
#define SWITCHEVENT_H

#include "Event.h"
#include "../Characters/Player.h"

class SwitchEvent : public Event {
private:
    DamageMode mode;

public:
    SwitchEvent(DamageMode mode);
    std::string toString() const override;
};

#endif