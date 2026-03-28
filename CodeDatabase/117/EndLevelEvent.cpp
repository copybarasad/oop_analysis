#include "EndLevelEvent.h"
#include <sstream>
#include <iomanip>

EndLevelEvent::EndLevelEvent(const std::string& levelName, bool win, int playerEXP):
    levelName(levelName), win(win), playerEXP(playerEXP) {} 

std::string EndLevelEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - Level '" << levelName << "' end. ";
    if (win) {
        result << " Player win ";
    }
    else{
        result << " Player lose ";
    }
    result << "with EXP " << playerEXP;
    
    return result.str();
}