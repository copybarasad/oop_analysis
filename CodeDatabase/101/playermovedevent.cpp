#include "playermovedevent.h"

PlayerMovedEvent::PlayerMovedEvent(int fromX, int fromY, int toX, int toY)
    : GameEvent(EventType::PLAYER_MOVED, LogLevel::INFO),
    fromX_(fromX), fromY_(fromY), toX_(toX), toY_(toY) {}

std::string PlayerMovedEvent::toString() const {
    std::ostringstream oss;
    oss << "Player moved from (" << fromX_ << "," << fromY_
        << ") to (" << toX_ << "," << toY_ << ")";
    return oss.str();
}
