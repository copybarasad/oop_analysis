#ifndef LEVELCOMPLETEDEVENT_H
#define LEVELCOMPLETEDEVENT_H

#include "gameevent.h"

class LevelCompletedEvent : public GameEvent {
public:
    LevelCompletedEvent();

    std::string toString() const override;
};

#endif
