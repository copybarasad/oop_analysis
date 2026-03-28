#include "CellSerializer.h"
#include "../Cell.h"

nlohmann::json CellSerializer::serialize(const Cell& cell) const {
    nlohmann::json json;
    json["type"] = static_cast<int>(cell.getType());
    json["trapDamage"] = cell.getTrapDamage();
    json["hasEntity"] = cell.getHasEntity();
    return json;
}

void CellSerializer::deserialize(Cell& cell, const nlohmann::json& json) const {
    if (!json.contains("type") || !json.contains("trapDamage") || !json.contains("hasEntity")) {
        throw std::runtime_error("Некорректный формат JSON для Cell: отсутствуют обязательные поля");
    }
    
    // Проверка типов данных
    if (!json["type"].is_number_integer() || !json["trapDamage"].is_number_integer() ||
        !json["hasEntity"].is_boolean()) {
        throw std::runtime_error("Некорректный тип данных в JSON для Cell");
    }
    
    int typeValue;
    int trapDamage;
    bool hasEntity;
    try {
        typeValue = json["type"].get<int>();
        trapDamage = json["trapDamage"].get<int>();
        hasEntity = json["hasEntity"].get<bool>();
    } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Ошибка чтения значений Cell: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw std::runtime_error("Переполнение при чтении значений Cell: " + std::string(e.what()));
    }
    
    // Валидация типа клетки
    if (typeValue < 0 || typeValue > 3) {
        throw std::runtime_error("Некорректный тип клетки: " + std::to_string(typeValue) + 
                                " (допустимые значения: 0-3)");
    }
    
    // Валидация урона ловушки
    const int MAX_TRAP_DAMAGE = 10000;
    if (trapDamage < 0) {
        throw std::runtime_error("Некорректный урон ловушки: не может быть отрицательным, получено " + 
                                std::to_string(trapDamage));
    }
    if (trapDamage > MAX_TRAP_DAMAGE) {
        throw std::runtime_error("Слишком большой урон ловушки: " + std::to_string(trapDamage) + 
                                " (максимум " + std::to_string(MAX_TRAP_DAMAGE) + ")");
    }
    
    cell.setType(static_cast<CellType>(typeValue));
    cell.setTrapDamage(trapDamage);
    cell.setHasEntity(hasEntity);
}

