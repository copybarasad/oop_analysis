#include "SwitchEvent.h"
#include <sstream>
#include <iomanip>

SwitchEvent::SwitchEvent(DamageMode mode) : mode(mode) {}

std::string SwitchEvent::toString() const{
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - Player switched damage mode on ";
    result << (this->mode == DamageMode::Melee ? "melee mode" : "range mode");
    return result.str();
}