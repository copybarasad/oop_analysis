#include "SpellCastEvent.h"
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

SpellCastEvent::SpellCastEvent(SpellType type, bool castSuccess, 
                               std::optional<int> x, std::optional<int> y)
    : GameEvent(EventType::SPELL_CAST, ""),
      spellType(type), targetX(x), targetY(y), success(castSuccess) {
}

std::string SpellCastEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] Игрок применил заклинание: " << spellTypeToString(spellType);
    if (targetX.has_value() && targetY.has_value()) {
        ss << " в позиции (" << *targetX << ", " << *targetY << ")";
    }
    ss << (success ? " - успешно" : " - неудачно");
    return ss.str();
}

