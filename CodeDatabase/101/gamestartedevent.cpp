#include "gamestartedevent.h"


GameStartedEvent::GameStartedEvent()
    : GameEvent(EventType::GAME_STARTED, LogLevel::INFO) {}


std::string GameStartedEvent::toString() const {
    return "START GAME";
}
