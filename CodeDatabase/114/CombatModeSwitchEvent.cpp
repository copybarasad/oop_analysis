#include "CombatModeSwitchEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

CombatModeSwitchEvent::CombatModeSwitchEvent(CombatMode mode)
    : GameEvent(EventType::COMBAT_MODE_SWITCHED, ""), newMode(mode) {
}

std::string CombatModeSwitchEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] Игрок переключил режим боя на: ";
    ss << (newMode == CombatMode::MELEE ? "Ближний бой" : "Дальний бой");
    return ss.str();
}

