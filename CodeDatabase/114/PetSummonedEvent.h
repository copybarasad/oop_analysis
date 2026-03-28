#ifndef PETSUMMONEDEVENT_H
#define PETSUMMONEDEVENT_H

#include "GameEvent.h"

class PetSummonedEvent : public GameEvent {
private:
    int x;
    int y;

public:
    PetSummonedEvent(int summonX, int summonY);
    
    std::string toString() const override;
    
    int getX() const { return x; }
    int getY() const { return y; }
};

#endif

