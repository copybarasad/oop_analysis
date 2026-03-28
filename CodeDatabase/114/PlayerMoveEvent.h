#ifndef PLAYERMOVEEVENT_H
#define PLAYERMOVEEVENT_H

#include "GameEvent.h"

class PlayerMoveEvent : public GameEvent {
private:
    int fromX;
    int fromY;
    int toX;
    int toY;

public:
    PlayerMoveEvent(int fromX, int fromY, int toX, int toY);
    
    std::string toString() const override;
    
    int getFromX() const { return fromX; }
    int getFromY() const { return fromY; }
    int getToX() const { return toX; }
    int getToY() const { return toY; }
};

#endif

