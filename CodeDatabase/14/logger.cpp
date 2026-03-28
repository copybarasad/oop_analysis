#include "logger.hpp"

GameLogger::GameLogger(std::ostream& stream) : outStream(stream) {}

void GameLogger::onNotify(EventType type, const std::string& message) {
    std::string prefix;
    switch (type) {
        case EventType::INFO:    prefix = "[INFO]    "; break;
        case EventType::DAMAGE:  prefix = "[DAMAGE]  "; break;
        case EventType::MOVE:    prefix = "[MOVE]    "; break;
        case EventType::DEATH:   prefix = "[DEATH]   "; break;
        case EventType::ERROR:   prefix = "[ERROR]   "; break;
        case EventType::LEVEL_UP:prefix = "[LEVEL UP]"; break;
        default:                 prefix = "[LOG]     "; break;
    }
    outStream << prefix << message << std::endl;
}