#include "EnemyBuildingSerializer.h"
#include "../entities/EnemyBuilding.h"
#include "EntitySerializer.h"

nlohmann::json EnemyBuildingSerializer::serialize(const EnemyBuilding& building) const {
    nlohmann::json json = entitySerializer.serialize(building);
    json["type"] = "EnemyBuilding";
    json["turnsToSpawn"] = building.getTurnsToSpawn();
    json["maxTurnsToSpawn"] = building.getMaxTurnsToSpawn();
    return json;
}

void EnemyBuildingSerializer::deserialize(EnemyBuilding& building, const nlohmann::json& json) const {
    if (!json.contains("type") || json["type"] != "EnemyBuilding") {
        throw std::runtime_error("Некорретный тип EnemyBuilding: не EnemyBuilding");
    }

    if (!json.contains("turnsToSpawn") || !json.contains("maxTurnsToSpawn")) {
        throw std::runtime_error("Некорректный формат JSON для EnemyBuilding: отсутствуют обязательные поля");
    }
    
    if (!json["turnsToSpawn"].is_number_integer() || !json["maxTurnsToSpawn"].is_number_integer()) {
        throw std::runtime_error("Некорректный тип данных для turnsToSpawn/maxTurnsToSpawn в EnemyBuilding");
    }
    
    entitySerializer.deserialize(building, json);
    
    int turnsToSpawn, maxTurnsToSpawn;
    try {
        turnsToSpawn = json["turnsToSpawn"].get<int>();
        maxTurnsToSpawn = json["maxTurnsToSpawn"].get<int>();
    } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Ошибка чтения значений EnemyBuilding: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw std::runtime_error("Переполнение при чтении значений EnemyBuilding: " + std::string(e.what()));
    }
    
    const int MAX_TURNS = 1000;
    if (turnsToSpawn < 0) {
        throw std::runtime_error("Некорректное значение turnsToSpawn: не может быть отрицательным, получено " + 
                                std::to_string(turnsToSpawn));
    }
    if (turnsToSpawn > MAX_TURNS) {
        throw std::runtime_error("Слишком большое значение turnsToSpawn: " + std::to_string(turnsToSpawn) + 
                                " (максимум " + std::to_string(MAX_TURNS) + ")");
    }
    
    if (maxTurnsToSpawn <= 0) {
        throw std::runtime_error("Некорректное значение maxTurnsToSpawn: должно быть больше 0, получено " + 
                                std::to_string(maxTurnsToSpawn));
    }
    if (maxTurnsToSpawn > MAX_TURNS) {
        throw std::runtime_error("Слишком большое значение maxTurnsToSpawn: " + std::to_string(maxTurnsToSpawn) + 
                                " (максимум " + std::to_string(MAX_TURNS) + ")");
    }
    
    if (turnsToSpawn > maxTurnsToSpawn) {
        throw std::runtime_error("turnsToSpawn (" + std::to_string(turnsToSpawn) + 
                                ") не может быть больше maxTurnsToSpawn (" + std::to_string(maxTurnsToSpawn) + ")");
    }
    
    building.setTurnsToSpawn(turnsToSpawn);
    building.setMaxTurnsToSpawn(maxTurnsToSpawn);
}

