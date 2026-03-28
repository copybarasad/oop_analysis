#include "EntitySerializer.h"
#include "../entities/Entity.h"

nlohmann::json EntitySerializer::serialize(const Entity& entity) const {
    nlohmann::json json;
    json["health"] = entity.getHealth();
    json["maxHealth"] = entity.getMaxHealth();
    json["x"] = entity.getX();
    json["y"] = entity.getY();
    return json;
}

void EntitySerializer::deserialize(Entity& entity, const nlohmann::json& json) const {
    if (!json.contains("health") || !json.contains("maxHealth") ||
        !json.contains("x") || !json.contains("y")) {
        throw std::runtime_error("Некорректный формат JSON для Entity: отсутствуют обязательные поля");
    }
    
    // Проверка типов данных
    if (!json["health"].is_number_integer() || !json["maxHealth"].is_number_integer() ||
        !json["x"].is_number_integer() || !json["y"].is_number_integer()) {
        throw std::runtime_error("Некорректный тип данных в JSON для Entity: ожидаются целые числа");
    }
    
    int health, maxHealth, x, y;
    try {
        health = json["health"].get<int>();
        maxHealth = json["maxHealth"].get<int>();
        x = json["x"].get<int>();
        y = json["y"].get<int>();
    } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Ошибка чтения числовых значений Entity: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw std::runtime_error("Переполнение при чтении значений Entity: " + std::string(e.what()));
    }
    
    // Строгая валидация значений
    const int MAX_HEALTH = 1000000;
    const int MAX_COORD = 1000;
    
    if (maxHealth <= 0) {
        throw std::runtime_error("Некорректное максимальное здоровье: должно быть больше 0, получено " + 
                                std::to_string(maxHealth));
    }
    
    if (maxHealth > MAX_HEALTH) {
        throw std::runtime_error("Слишком большое максимальное здоровье: " + 
                                std::to_string(maxHealth) + " (максимум " + std::to_string(MAX_HEALTH) + ")");
    }
    
    if (health < 0) {
        throw std::runtime_error("Некорректное здоровье: не может быть отрицательным, получено " + 
                                std::to_string(health));
    }
    
    if (health > maxHealth) {
        throw std::runtime_error("Некорректное здоровье: не может быть больше максимального (health=" + 
                                std::to_string(health) + ", maxHealth=" + std::to_string(maxHealth) + ")");
    }
    
    if (x < 0 || x > MAX_COORD) {
        throw std::runtime_error("Некорректная координата X: " + std::to_string(x) + 
                                " (допустимый диапазон: 0-" + std::to_string(MAX_COORD) + ")");
    }
    
    if (y < 0 || y > MAX_COORD) {
        throw std::runtime_error("Некорректная координата Y: " + std::to_string(y) + 
                                " (допустимый диапазон: 0-" + std::to_string(MAX_COORD) + ")");
    }
    
    entity.setHealth(health);
    entity.setMaxHealth(maxHealth);
    entity.setPosition(x, y);
}

