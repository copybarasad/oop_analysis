#ifndef SPELLBUYEVENT_H
#define SPELLBUYEVENT_H

#include "Event.h"

class SpellBuyEvent : public Event {
private:
    std::string spellName;
    int price;

public:
    SpellBuyEvent(const std::string& spellName, int price);
    std::string toString() const override;
};

#endif