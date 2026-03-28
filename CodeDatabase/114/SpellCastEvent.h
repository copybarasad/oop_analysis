#ifndef SPELLCASTEVENT_H
#define SPELLCASTEVENT_H

#include "GameEvent.h"
#include "../spells/ISpell.h"
#include <string>
#include <optional>

class SpellCastEvent : public GameEvent {
private:
    SpellType spellType;
    std::optional<int> targetX;
    std::optional<int> targetY;
    bool success;

public:
    SpellCastEvent(SpellType type, bool castSuccess, 
                   std::optional<int> x = std::nullopt, 
                   std::optional<int> y = std::nullopt);
    
    std::string toString() const override;
    
    SpellType getSpellType() const { return spellType; }
    bool wasSuccessful() const { return success; }
    std::optional<int> getTargetX() const { return targetX; }
    std::optional<int> getTargetY() const { return targetY; }
};

#endif

