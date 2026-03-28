#ifndef DAMAGEEVENT_H
#define DAMAGEEVENT_H

#include "GameEvent.h"
#include <string>

class DamageEvent : public GameEvent {
private:
    std::string attackerName;
    std::string targetName;
    int damage;
    int targetX;
    int targetY;

public:
    DamageEvent(const std::string& attacker, const std::string& target, 
                int damageAmount, int x, int y);
    
    std::string toString() const override;
    
    const std::string& getAttackerName() const { return attackerName; }
    const std::string& getTargetName() const { return targetName; }
    int getDamage() const { return damage; }
    int getTargetX() const { return targetX; }
    int getTargetY() const { return targetY; }
};

#endif

