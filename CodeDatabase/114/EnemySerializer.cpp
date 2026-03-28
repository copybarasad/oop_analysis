#include "EnemySerializer.h"
#include "../entities/Enemy.h"
#include "EntitySerializer.h"

nlohmann::json EnemySerializer::serialize(const Enemy& enemy) const {
    nlohmann::json json = entitySerializer.serialize(enemy);
    json["type"] = "Enemy";
    json["damage"] = enemy.getDamage();
    return json;
}

void EnemySerializer::deserialize(Enemy& enemy, const nlohmann::json& json) const {
    if (!json.contains("type") || json["type"] != "Enemy") {
        throw std::runtime_error("Некорретный тип Enemy: не Enemy");
    }

    if (!json.contains("damage")) {
        throw std::runtime_error("Некорректный формат JSON для Enemy: отсутствует поле damage");
    }
    
    if (!json["damage"].is_number_integer()) {
        throw std::runtime_error("Некорректный тип данных для damage в Enemy: ожидается целое число");
    }
    
    entitySerializer.deserialize(enemy, json);
    
    int damage;
    try {
        damage = json["damage"].get<int>();
    } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Ошибка чтения урона Enemy: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw std::runtime_error("Переполнение при чтении урона Enemy: " + std::string(e.what()));
    }
    
    const int MAX_DAMAGE = 10000;
    if (damage < 0) {
        throw std::runtime_error("Некорректное значение урона врага: не может быть отрицательным, получено " + 
                                std::to_string(damage));
    }
    if (damage > MAX_DAMAGE) {
        throw std::runtime_error("Слишком большой урон врага: " + std::to_string(damage) + 
                                " (максимум " + std::to_string(MAX_DAMAGE) + ")");
    }
    
    enemy.setDamage(damage);
}

