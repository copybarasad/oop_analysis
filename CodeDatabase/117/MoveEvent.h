#ifndef MOVEEVENT_H
#define MOVEEVENT_H

#include "Event.h"

class MoveEvent : public Event {
private:
    std::string characterName;
    std::pair<int, int> startPos;
    std::pair<int, int> finalPos;

public:
    MoveEvent(const std::string& characterName, std::pair<int, int> startPos, std::pair<int, int> finalPos);
    std::string toString() const override;
};

#endif