#include "LoadEvent.h"
#include <sstream>
#include <iomanip>

LoadEvent::LoadEvent(const std::string& levelName, int slotNumber, bool isLevel)
    : levelName(levelName), slotNumber(slotNumber), isLevel(isLevel) {}

std::string LoadEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    if (isLevel) {
        result << " - Game loaded from level '" << levelName << "'";
    } else {
        result << " - Game loaded from save slot " << slotNumber;
    }
    return result.str();
}