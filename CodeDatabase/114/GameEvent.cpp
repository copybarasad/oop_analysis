#include "GameEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

GameEvent::GameEvent(EventType eventType, const std::string& eventDescription)
    : type(eventType), description(eventDescription) {
    timestamp = std::time(nullptr);
}

std::string GameEvent::toString() const {
    std::stringstream ss;
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    if (!description.empty()) {
        ss << " [" << static_cast<int>(type) << "] " << description;
    } else {
        ss << " [" << static_cast<int>(type) << "]";
    }
    return ss.str();
}

