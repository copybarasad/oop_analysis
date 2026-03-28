#include "SpawnEvent.h"
#include <sstream>
#include <iomanip>

SpawnEvent::SpawnEvent(std::string spawner, std::string spawned, std::pair<int, int> spawnPos):
    spawner(spawner), spawned(spawned), spawnPos(spawnPos) {}

std::string SpawnEvent::toString() const{
    std::stringstream result;
    time_t eventTime = this->getTime();
    std::tm* time = std::localtime(&eventTime);
    result << std::put_time(time, "%d-%m-%Y %H:%M:%S");
    result << " - " << spawner << " spawned " << spawned;
    result << " at (" << spawnPos.first << ' ' << spawnPos.second << ")";
    return result.str();
}