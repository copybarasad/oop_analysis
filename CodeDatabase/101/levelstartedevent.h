#ifndef LEVELSTARTEDEVENT_H
#define LEVELSTARTEDEVENT_H

#include "gameevent.h"

class LevelStartedEvent : public GameEvent {
private:
    int level_;

public:
    explicit LevelStartedEvent(int level);

    std::string toString() const override;
    int getLevel() const { return level_; }
};

#endif
