#include "Serialization/JsonManager.hpp"

void JsonManager::marshal(const std::vector<SpellContract>& traps, const char* path) {
    nlohmann::json j = nlohmann::json::array();

    for (const auto& t : traps) {
        nlohmann::json obj;
        obj["damage"] = t.damage;
        obj["longLive"] = t.longLive;
        obj["coord"] = {
            {"x", t.coord.x},
            {"y", t.coord.y}
        };

        j.push_back(obj);
    }

    fileManager.write(path, j);
}
