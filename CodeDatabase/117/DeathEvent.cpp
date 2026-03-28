#include "DeathEvent.h"
#include <sstream>
#include <iomanip>

DeathEvent::DeathEvent(const std::string& characterName): characterName(characterName) {}

std::string DeathEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - " << characterName << " died";
    return result.str();
}