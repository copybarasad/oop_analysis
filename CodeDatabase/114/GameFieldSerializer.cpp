#include "GameFieldSerializer.h"
#include "../GameField.h"
#include "CellSerializer.h"
#include "EnemySerializer.h"
#include "EnemyBuildingSerializer.h"
#include "PetSerializer.h"
#include "PlayerSerializer.h"

nlohmann::json GameFieldSerializer::serialize(const GameField& field) const {
    nlohmann::json json;
    json["width"] = field.getWidth();
    json["height"] = field.getHeight();
    
    nlohmann::json fieldArray = nlohmann::json::array();
    for (const auto& row : field.getCells()) {
        nlohmann::json rowArray = nlohmann::json::array();
        for (const auto& cell : row) {
            rowArray.push_back(cellSerializer.serialize(cell));
        }
        fieldArray.push_back(rowArray);
    }
    json["field"] = fieldArray;
    
    nlohmann::json enemiesArray = nlohmann::json::array();
    for (const auto& enemy : field.getEnemies()) {
        enemiesArray.push_back(enemySerializer.serialize(enemy));
    }
    json["enemies"] = enemiesArray;
    
    nlohmann::json buildingsArray = nlohmann::json::array();
    for (const auto& building : field.getBuilding()) {
        buildingsArray.push_back(buildingSerializer.serialize(building));
    }
    json["buildings"] = buildingsArray;
    
    nlohmann::json petsArray = nlohmann::json::array();
    for (const auto& pet : field.getPets()) {
        petsArray.push_back(petSerializer.serialize(pet));
    }
    json["pets"] = petsArray;
    
    json["player"] = playerSerializer.serialize(field.getPlayer());
    
    return json;
}

void GameFieldSerializer::deserialize(GameField& field, const nlohmann::json& json) const {
    if (!json.contains("width") || !json.contains("height") || 
        !json.contains("field") || !json.contains("player") ||
        !json.contains("enemies") || !json.contains("buildings") || 
        !json.contains("pets")) {
        throw std::runtime_error("Некорректный формат JSON: отсутствуют обязательные поля");
    }
    
    // Проверка типов данных
    if (!json["width"].is_number_integer() || !json["height"].is_number_integer() ||
        !json["field"].is_array() || !json["player"].is_object() ||
        !json["enemies"].is_array() || !json["buildings"].is_array() ||
        !json["pets"].is_array()) {
        throw std::runtime_error("Некорректный тип данных в JSON для GameField");
    }
    
    int width, height;
    try {
        width = json["width"].get<int>();
        height = json["height"].get<int>();
    } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Ошибка чтения размеров поля: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw std::runtime_error("Переполнение при чтении размеров поля: " + std::string(e.what()));
    }
    
    // Строгая валидация размеров поля
    const int MIN_SIZE = 10;
    const int MAX_SIZE = 25;
    if (width < MIN_SIZE || width > MAX_SIZE) {
        throw std::runtime_error("Некорректная ширина поля: " + std::to_string(width) + 
                                " (допустимый диапазон: " + std::to_string(MIN_SIZE) + "-" + 
                                std::to_string(MAX_SIZE) + ")");
    }
    if (height < MIN_SIZE || height > MAX_SIZE) {
        throw std::runtime_error("Некорректная высота поля: " + std::to_string(height) + 
                                " (допустимый диапазон: " + std::to_string(MIN_SIZE) + "-" + 
                                std::to_string(MAX_SIZE) + ")");
    }
    
    // Поле уже создано с нужными размерами, просто проверяем что они совпадают
    // Если не совпадают, это ошибка - поле должно быть создано с правильными размерами перед десериализацией
    if (field.getWidth() != width || field.getHeight() != height) {
        throw std::runtime_error("Размеры поля не совпадают: поле имеет размеры " + 
                                std::to_string(field.getWidth()) + "x" + std::to_string(field.getHeight()) + 
                                ", но в JSON указаны " + std::to_string(width) + "x" + std::to_string(height));
    }
    
    // Загружаем клетки
    if (json["field"].size() != static_cast<size_t>(height)) {
        throw std::runtime_error("Некорректное количество строк в поле: ожидается " + 
                                std::to_string(height) + ", получено " + 
                                std::to_string(json["field"].size()));
    }
    
    auto& cells = field.getCells();
    for (size_t i = 0; i < json["field"].size(); ++i) {
        const auto& rowJson = json["field"][i];
        if (!rowJson.is_array()) {
            throw std::runtime_error("Некорректный тип данных для строки " + std::to_string(i) + " поля");
        }
        if (rowJson.size() != static_cast<size_t>(width)) {
            throw std::runtime_error("Некорректная длина строки " + std::to_string(i) + 
                                    ": ожидается " + std::to_string(width) + ", получено " + 
                                    std::to_string(rowJson.size()));
        }
        
        for (size_t j = 0; j < rowJson.size(); ++j) {
            cellSerializer.deserialize(cells[i][j], rowJson[j]);
        }
    }
    
    // Загружаем врагов
    auto& enemies = field.getEnemies();
    enemies.clear();
    for (size_t i = 0; i < json["enemies"].size(); ++i) {
        const auto& enemyJson = json["enemies"][i];
        if (!enemyJson.is_object()) {
            throw std::runtime_error("Некорректный тип данных для врага " + std::to_string(i));
        }
        Enemy enemy;
        enemySerializer.deserialize(enemy, enemyJson);
        // Проверяем, что враг находится в пределах поля
        if (!field.isValidPosition(enemy.getX(), enemy.getY())) {
            throw std::runtime_error("Враг " + std::to_string(i) + " находится вне границ поля: (" + 
                                    std::to_string(enemy.getX()) + ", " + std::to_string(enemy.getY()) + ")");
        }
        enemies.push_back(enemy);
    }
    
    // Загружаем здания
    auto& buildings = field.getBuilding();
    buildings.clear();
    for (size_t i = 0; i < json["buildings"].size(); ++i) {
        const auto& buildingJson = json["buildings"][i];
        if (!buildingJson.is_object()) {
            throw std::runtime_error("Некорректный тип данных для здания " + std::to_string(i));
        }
        EnemyBuilding building(0, 0);
        buildingSerializer.deserialize(building, buildingJson);
        // Проверяем, что здание находится в пределах поля
        if (!field.isValidPosition(building.getX(), building.getY())) {
            throw std::runtime_error("Здание " + std::to_string(i) + " находится вне границ поля: (" + 
                                    std::to_string(building.getX()) + ", " + std::to_string(building.getY()) + ")");
        }
        buildings.push_back(building);
    }
    
    // Загружаем союзников
    auto& pets = field.getPets();
    pets.clear();
    for (size_t i = 0; i < json["pets"].size(); ++i) {
        const auto& petJson = json["pets"][i];
        if (!petJson.is_object()) {
            throw std::runtime_error("Некорректный тип данных для союзника " + std::to_string(i));
        }
        Pet pet;
        petSerializer.deserialize(pet, petJson);
        // Проверяем, что союзник находится в пределах поля
        if (!field.isValidPosition(pet.getX(), pet.getY())) {
            throw std::runtime_error("Союзник " + std::to_string(i) + " находится вне границ поля: (" + 
                                    std::to_string(pet.getX()) + ", " + std::to_string(pet.getY()) + ")");
        }
        pets.push_back(pet);
    }
    
    // Загружаем игрока
    playerSerializer.deserialize(field.getPlayer(), json["player"]);
    
    // Проверяем, что игрок находится в пределах поля
    if (!field.isValidPosition(field.getPlayer().getX(), field.getPlayer().getY())) {
        throw std::runtime_error("Игрок находится вне границ поля: (" + 
                                std::to_string(field.getPlayer().getX()) + ", " + 
                                std::to_string(field.getPlayer().getY()) + ")");
    }
    
    // Обновляем флаги hasEntity
    for (auto& row : cells) {
        for (auto& cell : row) {
            cell.setHasEntity(false);
        }
    }
    
    cells[field.getPlayer().getY()][field.getPlayer().getX()].setHasEntity(true);
    
    for (auto& enemy : enemies) {
        if (field.isValidPosition(enemy.getX(), enemy.getY())) {
            cells[enemy.getY()][enemy.getX()].setHasEntity(true);
        }
    }
    
    for (auto& building : buildings) {
        if (field.isValidPosition(building.getX(), building.getY())) {
            cells[building.getY()][building.getX()].setHasEntity(true);
        }
    }
    
    for (auto& pet : pets) {
        if (field.isValidPosition(pet.getX(), pet.getY())) {
            cells[pet.getY()][pet.getX()].setHasEntity(true);
        }
    }
}

