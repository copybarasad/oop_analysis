#include "MoveEvent.h"
#include <sstream>
#include <iomanip>

MoveEvent::MoveEvent(const std::string& characterName, std::pair<int, int> startPos, std::pair<int, int> finalPos)
    : characterName(characterName), startPos(startPos), finalPos(finalPos) {}

std::string MoveEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - " << characterName << " moved from (" << startPos.first << ", " << startPos.second << ")";
    result << " to (" << finalPos.first << ", " << finalPos.second << ")";
    return result.str();
}