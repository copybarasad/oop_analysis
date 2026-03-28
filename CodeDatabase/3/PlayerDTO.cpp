#include "Serialization/JsonManager.hpp"

void JsonManager::marshal(const PlayerDTO& player, const char* path) {
    nlohmann::json j;

    j["maxHP"] = player.maxHP;
    j["hp"] = player.hp;
    j["dmg"] = player.dmg;
    j["disabled"] = player.disabled;
    j["rangeForm"] = player.rangeForm;
    
    j["coord"] = {
        {"x", player.coord.x},
        {"y", player.coord.y}
    };

    fileManager.write(path, j);
}
