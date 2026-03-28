#include "towerdestroyedevent.h"

TowerDestroyedEvent::TowerDestroyedEvent()
    : GameEvent(EventType::TOWER_DESTROYED, LogLevel::INFO) {}

std::string TowerDestroyedEvent::toString() const {
    return "Tower is destroyed!";
}
