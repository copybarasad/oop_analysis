#ifndef TOWERDESTROYEDEVENT_H
#define TOWERDESTROYEDEVENT_H

#include "gameevent.h"

class TowerDestroyedEvent : public GameEvent {
public:
    TowerDestroyedEvent();

    std::string toString() const override;
};

#endif
