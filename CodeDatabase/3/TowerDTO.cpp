#include "Serialization/JsonManager.hpp"

void JsonManager::marshal(const std::vector<TowerDTO>& towers, const char* path) {
    nlohmann::json j = nlohmann::json::array();

    for (const auto& t : towers) {
        nlohmann::json obj;

        obj["coolDown"] = t.coolDown;
        obj["steps"] = t.steps;
        obj["coord"] = {
            { "x", t.coord.x },
            { "y", t.coord.y }
        };
        obj["isAttack"] = t.isAttack;
        if (t.isAttack) {
            obj["hp"] = t.hp;
            obj["maxHp"] = t.maxHp;
        } else {
            obj["enemyStats"] = {
                { "maxHP", t.enemyStats.maxHP },
                { "hp",    t.enemyStats.hp },
                { "dmg",   t.enemyStats.dmg }
            };
        }

        j.push_back(obj);
    }

    fileManager.write(path, j);
}
