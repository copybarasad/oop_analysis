#include "levelcompletedevent.h"

LevelCompletedEvent::LevelCompletedEvent()
    : GameEvent(EventType::LEVEL_COMPLETED, LogLevel::INFO) {}

std::string LevelCompletedEvent::toString() const {
    return "Level completed!";
}
