#ifndef SPELLCASTEVENT_H
#define SPELLCASTEVENT_H

#include "Event.h"

class SpellCastEvent : public Event {
private:
    std::string spellName;
    std::string casterName;

public:
    SpellCastEvent(const std::string& spellName, const std::string& casterName);
    std::string toString() const override;
};

#endif