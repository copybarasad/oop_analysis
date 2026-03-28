#ifndef TRAPTRIGGEREDEVENT_H
#define TRAPTRIGGEREDEVENT_H

#include "GameEvent.h"
#include <string>

class TrapTriggeredEvent : public GameEvent {
private:
    std::string victimName;
    int damage;
    int x;
    int y;

public:
    TrapTriggeredEvent(const std::string& victim, int trapDamage, int trapX, int trapY);
    
    std::string toString() const override;
    
    const std::string& getVictimName() const { return victimName; }
    int getDamage() const { return damage; }
    int getX() const { return x; }
    int getY() const { return y; }
};

#endif

