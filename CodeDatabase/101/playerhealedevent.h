#ifndef PLAYERHEALEDEVENT_H
#define PLAYERHEALEDEVENT_H

#include "gameevent.h"

class PlayerHealedEvent : public GameEvent {
private:
    int amount_;

public:
    explicit PlayerHealedEvent(int amount);

    std::string toString() const override;
    int getAmount() const { return amount_; }
};

#endif
