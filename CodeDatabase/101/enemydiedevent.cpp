#include "enemydiedevent.h"

EnemyDiedEvent::EnemyDiedEvent(const std::string& enemyType, int x, int y)
    : GameEvent(EventType::ENEMY_DIED, LogLevel::INFO),
    enemyType_(enemyType), x_(x), y_(y) {}

std::string EnemyDiedEvent::toString() const {
    std::ostringstream oss;
    oss << enemyType_ << " died at (" << x_ << "," << y_ << ")";
    return oss.str();
}
