#include "spellcastevent.h"

SpellCastEvent::SpellCastEvent(const std::string& spellName, int cost, int x, int y)
    : GameEvent(EventType::SPELL_CAST, LogLevel::INFO),
    spellName_(spellName), cost_(cost), x_(x), y_(y) {}

std::string SpellCastEvent::toString() const {
    std::ostringstream oss;
    oss << "Spell '" << spellName_ << "' cast for " << cost_
        << " money at (" << x_ << "," << y_ << ")";
    return oss.str();
}
