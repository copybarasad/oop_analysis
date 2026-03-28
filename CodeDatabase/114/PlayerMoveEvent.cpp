#include "PlayerMoveEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

PlayerMoveEvent::PlayerMoveEvent(int fromX, int fromY, int toX, int toY)
    : GameEvent(EventType::PLAYER_MOVED, ""),
      fromX(fromX), fromY(fromY), toX(toX), toY(toY) {
}

std::string PlayerMoveEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] Игрок переместился с (" << fromX << ", " << fromY 
       << ") на (" << toX << ", " << toY << ")";
    return ss.str();
}

