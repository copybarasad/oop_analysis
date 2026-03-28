#ifndef SPELLCASTEVENT_H
#define SPELLCASTEVENT_H
#include "gameevent.h"

class SpellCastEvent : public GameEvent {
private:
    std::string spellName_;
    int cost_;
    int x_, y_;

public:
    SpellCastEvent(const std::string& spellName, int cost, int x, int y);

    std::string toString() const override;
};

#endif // SPELLCASTEVENT_H
