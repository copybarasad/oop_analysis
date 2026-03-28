#include "playerattackedevent.h"

PlayerAttackedEvent::PlayerAttackedEvent(const std::string& target, int damage, int x, int y)
    : GameEvent(EventType::PLAYER_ATTACKED, LogLevel::INFO),
    target_(target), damage_(damage), x_(x), y_(y) {}

std::string PlayerAttackedEvent::toString() const {
    std::ostringstream oss;
    oss << "Player attacked " << target_ << " for " << damage_
        << " damage at (" << x_ << "," << y_ << ")";
    return oss.str();
}
