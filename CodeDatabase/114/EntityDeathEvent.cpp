#include "EntityDeathEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

EntityDeathEvent::EntityDeathEvent(const std::string& name, int deathX, int deathY)
    : GameEvent(EventType::ENTITY_DEATH, ""),
      entityName(name), x(deathX), y(deathY) {
}

std::string EntityDeathEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] " << entityName << " погиб в позиции (" << x << ", " << y << ")";
    return ss.str();
}

