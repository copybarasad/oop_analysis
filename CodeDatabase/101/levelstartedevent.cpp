#include "levelstartedevent.h"
#include <sstream>

LevelStartedEvent::LevelStartedEvent(int level)
    : GameEvent(EventType::LEVEL_STARTED, LogLevel::INFO), level_(level) {}

std::string LevelStartedEvent::toString() const {
    std::stringstream ss;
    ss << "Level " << level_ << " start";
    return ss.str();
}
