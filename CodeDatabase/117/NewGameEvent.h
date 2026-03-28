#ifndef NEWGAMEEVENT_H
#define NEWGAMEEVENT_H

#include "Event.h"

class NewGameEvent : public Event {
public:
    std::string toString() const override;
};

#endif