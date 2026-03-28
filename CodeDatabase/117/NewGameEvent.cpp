#include "NewGameEvent.h"
#include <sstream>
#include <iomanip>

std::string NewGameEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - Player start new game";
    return result.str();
}