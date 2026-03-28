#include "enemydamageevent.h"
#include <sstream>

EnemyDamageEvent::EnemyDamageEvent(int damage)
    : GameEvent(EventType::ENEMY_DAMAGED, LogLevel::INFO), damage_(damage) {}

std::string EnemyDamageEvent::toString() const {
    std::stringstream ss;
    ss << "Enemy received " << damage_ << " damage";
    return ss.str();
}
