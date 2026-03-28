#ifndef SCORECHANGEDEVENT_H
#define SCORECHANGEDEVENT_H

#include "gameevent.h"

class ScoreChangedEvent : public GameEvent {
private:
    int newScore_;

public:
    explicit ScoreChangedEvent(int newScore);

    std::string toString() const override;
    int getNewScore() const { return newScore_; }
};

#endif
