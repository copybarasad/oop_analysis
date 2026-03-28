#include "SaveEvent.h"
#include <sstream>
#include <iomanip>

SaveEvent::SaveEvent(int slotNumber): slotNumber(slotNumber) {}

std::string SaveEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - Game saved in slot " << slotNumber;
    return result.str();
}