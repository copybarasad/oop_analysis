#include "SaveLoadManager.h"
#include "SaveLoadException.h"
#include "../models/serializers/GameFieldSerializer.h"
#include <iostream>

SaveLoadManager::SaveLoadManager() = default;

void SaveLoadManager::saveGame(const std::string& filename, int level, const GameField& field) const {
    GameFieldSerializer serializer;
    nlohmann::json json;
    json["level"] = level;
    json["field"] = serializer.serialize(field);
    fileController.saveJson(filename, json);
}

void SaveLoadManager::loadGame(const std::string& filename, int& level, GameField& field) {
    nlohmann::json json = fileController.loadJson(filename);
    validateSaveFile(json);

    // Проверка типа данных для level
    if (!json["level"].is_number_integer()) {
        throw LoadException("Некорректный тип данных для уровня: ожидается целое число");
    }
    
    int levelValue;
    try {
        levelValue = json["level"].get<int>();
    } catch (const nlohmann::json::type_error& e) {
        throw LoadException("Ошибка чтения уровня: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw LoadException("Переполнение при чтении уровня: " + std::string(e.what()));
    }
    
    if (levelValue < 1 || levelValue > 100) {
        throw LoadException("Некорректный уровень: " + std::to_string(levelValue) + 
                           " (допустимый диапазон: 1-100)");
    }
    level = levelValue;

    // Проверка типа данных для field
    if (!json["field"].is_object()) {
        throw LoadException("Некорректный тип данных для поля: ожидается объект");
    }

    try {
        GameFieldSerializer serializer;
        serializer.deserialize(field, json["field"]);
    } catch (const std::exception& e) {
        throw LoadException(std::string("Ошибка загрузки игрового поля: ") + e.what());
    }
}

void SaveLoadManager::validateSaveFile(const nlohmann::json& json) const {
    if (!json.is_object()) {
        throw LoadException("Некорректный формат файла сохранения: корневой элемент должен быть объектом");
    }
    
    if (!json.contains("level") || !json.contains("field")) {
        throw LoadException("Некорректный формат файла сохранения: отсутствуют обязательные поля");
    }
}

std::string SaveLoadManager::getSaveFileName() const {
    std::string filename;
    std::cout << "Введите имя файла для сохранения (без расширения): ";
    std::cin >> filename;
    if (filename.empty()) {
        filename = "save";
    }
    return filename + ".json";
}

std::string SaveLoadManager::getSaveFileNameFromUser() const {
    return getSaveFileName();
}



