#ifndef COMBATMODESWITCHEVENT_H
#define COMBATMODESWITCHEVENT_H

#include "GameEvent.h"
#include "../entities/Player.h"

class CombatModeSwitchEvent : public GameEvent {
private:
    CombatMode newMode;

public:
    CombatModeSwitchEvent(CombatMode mode);
    
    std::string toString() const override;
    
    CombatMode getNewMode() const { return newMode; }
};

#endif

