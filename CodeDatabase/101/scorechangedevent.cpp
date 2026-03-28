#include "scorechangedevent.h"
#include <sstream>

ScoreChangedEvent::ScoreChangedEvent(int newScore)
    : GameEvent(EventType::SCORE_CHANGED, LogLevel::INFO), newScore_(newScore) {}

std::string ScoreChangedEvent::toString() const {
    std::stringstream ss;
    ss << "Score changed: " << newScore_;
    return ss.str();
}
