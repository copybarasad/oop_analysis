#include "PetSummonedEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

PetSummonedEvent::PetSummonedEvent(int summonX, int summonY)
    : GameEvent(EventType::PET_SUMMONED, ""),
      x(summonX), y(summonY) {
}

std::string PetSummonedEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] Союзник призван в позиции (" << x << ", " << y << ")";
    return ss.str();
}

