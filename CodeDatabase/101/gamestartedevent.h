
#ifndef GAMESTARTEDEVENT_H
#define GAMESTARTEDEVENT_H

#include "gameevent.h"

class GameStartedEvent : public GameEvent {
public:
    GameStartedEvent();

    std::string toString() const override;
};

#endif
