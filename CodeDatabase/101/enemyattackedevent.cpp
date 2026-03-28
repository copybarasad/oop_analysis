#include "enemyattackedevent.h"
#include <sstream>

EnemyAttackedEvent::EnemyAttackedEvent(int damage)
    : GameEvent(EventType::ENEMY_ATTACKED, LogLevel::INFO), damage_(damage) {}

std::string EnemyAttackedEvent::toString() const {
    std::stringstream ss;
    ss << "Enemy attacked on " << damage_ << " damage";
    return ss.str();
}
