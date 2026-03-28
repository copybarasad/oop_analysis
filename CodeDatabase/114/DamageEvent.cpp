#include "DamageEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

DamageEvent::DamageEvent(const std::string& attacker, const std::string& target, 
                         int damageAmount, int x, int y)
    : GameEvent(EventType::DAMAGE_DEALT, ""),
      attackerName(attacker), targetName(target), damage(damageAmount),
      targetX(x), targetY(y) {
}

std::string DamageEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] " << attackerName << " нанес " << damage 
       << " урона " << targetName << " в позиции (" << targetX << ", " << targetY << ")";
    return ss.str();
}

