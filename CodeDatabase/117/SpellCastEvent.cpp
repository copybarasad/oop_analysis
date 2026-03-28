#include "SpellCastEvent.h"
#include <sstream>
#include <iomanip>

SpellCastEvent::SpellCastEvent(const std::string& spellName, const std::string& casterName):
    spellName(spellName), casterName(casterName) {}

std::string SpellCastEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - " << casterName << " cast spell " << spellName;
    return result.str();
}