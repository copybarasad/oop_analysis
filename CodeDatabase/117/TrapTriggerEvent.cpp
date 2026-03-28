#include "TrapTriggerEvent.h"
#include <sstream>
#include <iomanip>

TrapTriggerEvent::TrapTriggerEvent(const std::string& target, int damage, std::pair<int, int> position):
    target(target), damage(damage), position(position) {}

std::string TrapTriggerEvent::toString() const{
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - Trap worked on " << this->target;
    result << " at (" << this->position.first << " " << this->position.second << ") ";
    result << "with damage " << this->damage;
    return result.str();
}