#include "SpellObtainedEvent.h"
#include <sstream>
#include <map>
#include <iomanip>
#include <ctime>

namespace {
    std::string spellTypeToString(SpellType type) {
        static const std::map<SpellType, std::string> spellNames = {
            {SpellType::DIRECT_DAMAGE, "Прямой урон"},
            {SpellType::AREA_DAMAGE, "Урон по площади"},
            {SpellType::TRAP, "Ловушка"},
            {SpellType::IMPROVE, "Улучшение"},
            {SpellType::SUMMON, "Призыв"}
        };
        auto it = spellNames.find(type);
        return (it != spellNames.end()) ? it->second : "Неизвестное заклинание";
    }
}

SpellObtainedEvent::SpellObtainedEvent(SpellType type, const std::string& obtainMethod)
    : GameEvent(EventType::SPELL_OBTAINED, ""),
      spellType(type), method(obtainMethod) {
}

std::string SpellObtainedEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] Игрок получил заклинание: " << spellTypeToString(spellType);
    if (!method.empty()) {
        ss << " (способ получения: " << method << ")";
    }
    return ss.str();
}

