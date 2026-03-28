#include "TrapTriggeredEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

TrapTriggeredEvent::TrapTriggeredEvent(const std::string& victim, int trapDamage, 
                                       int trapX, int trapY)
    : GameEvent(EventType::TRAP_TRIGGERED, ""),
      victimName(victim), damage(trapDamage), x(trapX), y(trapY) {
}

std::string TrapTriggeredEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] Ловушка сработала на " << victimName 
       << " в позиции (" << x << ", " << y << "), нанесено " << damage << " урона";
    return ss.str();
}

