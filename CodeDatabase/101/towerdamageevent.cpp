#include "towerdamageevent.h"
#include <sstream>

TowerDamageEvent::TowerDamageEvent(int damage)
    : GameEvent(EventType::TOWER_DAMAGED, LogLevel::INFO), damage_(damage) {}

std::string TowerDamageEvent::toString() const {
    std::stringstream ss;
    ss << "Tower received " << damage_ << " damage";
    return ss.str();
}
