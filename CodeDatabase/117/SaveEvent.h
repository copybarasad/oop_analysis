#ifndef SAVEEVENT_H
#define SAVEEVENT_H

#include "Event.h"

class SaveEvent : public Event {
private:
    int slotNumber;

public:
    SaveEvent(int slotNumber);
    std::string toString() const override;
};

#endif