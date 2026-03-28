//
// Created by Artem on 24.10.2025.
//

#include "SlowingCellEvent.h"

void SlowingCellEvent::impactOnCreatureInCell(Creature &creature) {
    creature.setDisabledFlag(true);
}

std::unique_ptr<CellEvent> SlowingCellEvent::clone() const {
    return std::make_unique<SlowingCellEvent>(*this);
}

std::string SlowingCellEvent::serialize() {
    return "{}";
}

SlowingCellEvent* SlowingCellEvent::deserialize() noexcept {
    return new SlowingCellEvent();
}
