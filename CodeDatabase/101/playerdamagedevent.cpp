#include "playerdamagedevent.h"

PlayerDamagedEvent::PlayerDamagedEvent(int damage, const std::string& source)
    : GameEvent(EventType::PLAYER_DAMAGED, LogLevel::WARN),
    damage_(damage), source_(source) {}

std::string PlayerDamagedEvent::toString() const {
    std::ostringstream oss;
    oss << "Player received " << damage_ << " damage from " << source_;
    return oss.str();
}
