#include "cardreceivedevent.h"

CardReceivedEvent::CardReceivedEvent(const std::string& cardName)
    : GameEvent(EventType::PLAYER_SPELL_RECEIVED, LogLevel::INFO),
    cardName_(cardName) {}

std::string CardReceivedEvent::toString() const {
    std::ostringstream oss;
    oss << "Received card: " << cardName_;
    return oss.str();
}

