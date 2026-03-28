#include "LoggingObserver.h"
#include <sstream>

LoggingObserver::LoggingObserver(std::shared_ptr<ILogger> logger)
    : logger_(std::move(logger)) {}

void LoggingObserver::OnGameEvent(const GameEvent& event) {
    std::ostringstream ss;

    switch (event.type) {
        case GameEventType::DamageDealt:
            ss << "[DAMAGE] " << event.source << " -> " << event.target
               << " amount=" << event.amount;
        break;
        case GameEventType::PlayerMoved:
            ss << "[MOVE] Player to (" << event.x << "," << event.y << ")";
        break;
        case GameEventType::SpellReceived:
            ss << "[SPELL] " << event.target << " received spell: " << event.extra;
        break;
        case GameEventType::EnemySpawned:
            ss << "[SPAWN] Enemy " << event.target << " at (" << event.x << "," << event.y << ")";
        break;
        case GameEventType::EnemyDied:
            ss << "[ENEMY_DIED] " << event.target;
        break;
        default:
            ss << "[EVENT] " << event.extra;
        break;
    }

    logger_->Log(ss.str());
}
