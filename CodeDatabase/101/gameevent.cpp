#include "gameevent.h"


GameEvent::GameEvent(EventType type, LogLevel level)
    : type_(type), level_(level), timestamp_(std::time(nullptr)) {}



EventType GameEvent::getType() const { return type_; }
LogLevel GameEvent::getLevel() const { return level_; }
std::time_t GameEvent::getTimestamp() const { return timestamp_; }

std::string GameEvent::toFormattedString() const {
    return formatTime() + " [" + levelToString() + "] " + toString();
}


std::string GameEvent::formatTime() const {
    char buffer[80];
    std::tm* timeinfo = std::localtime(&timestamp_);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

std::string GameEvent::levelToString() const {
    switch(level_) {
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARN: return "WARNING";
    case LogLevel::ERR: return "ERROR";
    default: return "UNKNOWN";
    }
}

