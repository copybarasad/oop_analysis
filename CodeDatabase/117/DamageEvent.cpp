#include "DamageEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

DamageEvent::DamageEvent(const std::string& attackerName, const std::string& targetName, int damage):
    attackerName(attackerName), targetName(targetName), damage(damage) {}

std::string DamageEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - " << attackerName << " dealt ";
    result << damage << " damage to " << targetName;
    return result.str();
}