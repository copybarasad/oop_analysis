#include "Serialization/JsonManager.hpp"

void JsonManager::marshal(const MapDTO& map, const char* path) {
    nlohmann::json j;

    j["width"] = map.width;
    j["height"] = map.height;

    j["FPs"] = nlohmann::json::array();

    for (const auto& fp : map.FPs) {
        nlohmann::json field;
        field["coord"] = { {"x", fp.coord.x}, {"y", fp.coord.y} };
        
        std::string type;
        switch (fp.type) {
            case FieldType::PLAYER:
                type = "PLAYER";
                break;

            case FieldType::ENEMY:
                type = "ENEMY";
                break;

            case FieldType::ATTACK_TOWER:
                type = "ATTACK_TOWER";
                break;

            case FieldType::SPAWN_TOWER:
                type = "SPAWN_TOWER";
                break;

            case FieldType::FIELD_DEFAULT:
                type = "FIELD_DEFAULT";
                break;

            case FieldType::FIELD_BLOCK:
                type = "FIELD_BLOCK";
                break;

            case FieldType::FIELD_TRAP:
                type = "FIELD_TRAP";
                break;

            case FieldType::FIELD_SLOW:
                type = "FIELD_SLOW";
                break;
        }
        field["type"] = type;
        
        j["FPs"].push_back(field);
    }

    fileManager.write(path, j);
}
