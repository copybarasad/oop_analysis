#include "playerhealedevent.h"
#include <sstream>

PlayerHealedEvent::PlayerHealedEvent(int amount)
    : GameEvent(EventType::PLAYER_HEALED, LogLevel::INFO), amount_(amount) {}

std::string PlayerHealedEvent::toString() const {
    std::stringstream ss;
    ss << "Player restored" << amount_ << " health";
    return ss.str();
}
