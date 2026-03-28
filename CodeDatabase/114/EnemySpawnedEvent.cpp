#include "EnemySpawnedEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

EnemySpawnedEvent::EnemySpawnedEvent(int spawnX, int spawnY, const std::string& spawnSource)
    : GameEvent(EventType::ENEMY_SPAWNED, ""),
      x(spawnX), y(spawnY), source(spawnSource) {
}

std::string EnemySpawnedEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] Враг создан в позиции (" << x << ", " << y << ")";
    if (!source.empty()) {
        ss << " источником: " << source;
    }
    return ss.str();
}

