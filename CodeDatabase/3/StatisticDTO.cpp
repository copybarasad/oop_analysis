#include "Serialization/JsonManager.hpp"

void JsonManager::marshal(const StatisticDTO& stat, const char* path) {
    nlohmann::json j;

    j["lvl"] = stat.lvl;
    j["points"] = stat.points;
    j["endLevelPoints"] = stat.endLevelPoints;
    

    fileManager.write(path, j);
}
