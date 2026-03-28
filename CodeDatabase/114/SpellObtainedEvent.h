#ifndef SPELLOBTAINEDEVENT_H
#define SPELLOBTAINEDEVENT_H

#include "GameEvent.h"
#include "../spells/ISpell.h"
#include <string>

class SpellObtainedEvent : public GameEvent {
private:
    SpellType spellType;
    std::string method;

public:
    SpellObtainedEvent(SpellType type, const std::string& obtainMethod);
    
    std::string toString() const override;
    
    SpellType getSpellType() const { return spellType; }
    const std::string& getMethod() const { return method; }
};

#endif

