#ifndef LOADEVENT_H
#define LOADEVENT_H

#include "Event.h"

class LoadEvent : public Event {
private:
    std::string levelName;
    int slotNumber;
    bool isLevel;

public:
    LoadEvent(const std::string& levelName, int slotNumber, bool isLevel);
    std::string toString() const override;
};

#endif