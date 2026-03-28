//
// Created by Artem on 24.10.2025.
//

#include "TrapCellEvent.h"

void TrapCellEvent::impactOnCreatureInCell(Creature &creature) {
    creature.changeHealthPoints((-1) * this->damage);
}

std::unique_ptr<CellEvent> TrapCellEvent::clone() const {
    return std::make_unique<TrapCellEvent>(*this);
}

std::string TrapCellEvent::serialize() {
    std::string res = "{";
    res += "damage:";
    res += std::to_string(this->damage) + "}";
    return res;
}

TrapCellEvent* TrapCellEvent::deserialize(std::map<std::string, std::string> fields) noexcept {
    int damage = std::stoi(fields.at("damage"));
    return new TrapCellEvent(damage);
}
