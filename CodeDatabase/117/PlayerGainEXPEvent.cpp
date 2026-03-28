#include "PlayerGainEXPEvent.h"
#include <sstream>
#include <iomanip>

PlayerGainEXPEvent::PlayerGainEXPEvent(int expGained): expGained(expGained) {}

std::string PlayerGainEXPEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - Player gained " << expGained << " EXP";
    return result.str();
}