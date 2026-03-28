#ifndef PLAYERGAINEXPEVENT_H
#define PLAYERGAINEXPEVENT_H

#include "Event.h"

class PlayerGainEXPEvent : public Event {
private:
    int expGained;

public:
    PlayerGainEXPEvent(int expGained);
    std::string toString() const override;
};

#endif