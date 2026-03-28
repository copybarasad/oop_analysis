#include "include/Spell.h"
#include "../gamecore/include/Manager.h"
#include <iostream>

std::optional<Position> Spell::getCasterPosIfValid(const Manager& manager, EntityId casterId) const {
    if (!hasCharges()) return std::nullopt;
    return manager.entities().positionOf(casterId);
}

void Spell::save(std::ostream& os) const {
    os << static_cast<int>(type_) << " " << charges_ << "\n";
}

void Spell::load(std::istream& is) {
    is >> charges_;
}
