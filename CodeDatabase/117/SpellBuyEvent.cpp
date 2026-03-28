#include "SpellBuyEvent.h"
#include <sstream>
#include <iomanip>

SpellBuyEvent::SpellBuyEvent(const std::string& spellName, int price) : spellName(spellName), price(price) {}

std::string SpellBuyEvent::toString() const {
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - Player purchased spell " << spellName;
    result << " for " << price << " EXP";
    return result.str();
}