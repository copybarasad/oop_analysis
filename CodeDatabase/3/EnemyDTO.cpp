#include "Serialization/JsonManager.hpp"

void JsonManager::marshal(const std::vector<EnemyDTO>& enemies, const char* path) {
    nlohmann::json j = nlohmann::json::array();

    for (const auto& enemy : enemies) {
        nlohmann::json obj;

        obj["maxHp"] = enemy.maxHP;
        obj["hp"] = enemy.hp;
        obj["dmg"] = enemy.dmg;
        obj["disabled"] = enemy.disabled;

        obj["coord"] = {
            {"x", enemy.coord.x},
            {"y", enemy.coord.y}
        };

        j.push_back(obj);
    }

    fileManager.write(path, j);
}
