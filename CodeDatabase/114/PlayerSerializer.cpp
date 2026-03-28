#include "PlayerSerializer.h"
#include "../entities/Player.h"
#include "EntitySerializer.h"
#include "HandSerializer.h"

nlohmann::json PlayerSerializer::serialize(const Player& player) const {
    nlohmann::json json = entitySerializer.serialize(player);
    json["type"] = "Player";
    json["meleeDamage"] = player.getMeleeDamage();
    json["rangedDamage"] = player.getRangedDamage();
    json["combatMode"] = static_cast<int>(player.getCombatMode());
    json["canMove"] = player.getCanMove();
    json["money"] = player.getMoney();
    json["hand"] = handSerializer.serialize(player.getHand());
    return json;
}

void PlayerSerializer::deserialize(Player& player, const nlohmann::json& json) const {
    if (!json.contains("type") || json["type"] != "Player") {
        throw std::runtime_error("Некорретный тип Player: не Player");
    }

    if (!json.contains("meleeDamage") || !json.contains("rangedDamage") ||
        !json.contains("combatMode") || !json.contains("canMove") ||
        !json.contains("money") || !json.contains("hand")) {
        throw std::runtime_error("Некорректный формат JSON для Player: отсутствуют обязательные поля");
    }
    
    // Проверка типов данных
    if (!json["meleeDamage"].is_number_integer() || !json["rangedDamage"].is_number_integer() ||
        !json["combatMode"].is_number_integer() || !json["canMove"].is_boolean() ||
        !json["money"].is_number_integer()) {
        throw std::runtime_error("Некорректный тип данных в JSON для Player");
    }
    
    entitySerializer.deserialize(player, json);
    
    int meleeDamage, rangedDamage;
    try {
        meleeDamage = json["meleeDamage"].get<int>();
        rangedDamage = json["rangedDamage"].get<int>();
    } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Ошибка чтения значений урона Player: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw std::runtime_error("Переполнение при чтении значений урона Player: " + std::string(e.what()));
    }
    
    // Строгая валидация урона
    const int MAX_DAMAGE = 10000;
    if (meleeDamage < 0) {
        throw std::runtime_error("Некорректный урон ближнего боя: не может быть отрицательным, получено " + 
                                std::to_string(meleeDamage));
    }
    if (meleeDamage > MAX_DAMAGE) {
        throw std::runtime_error("Слишком большой урон ближнего боя: " + std::to_string(meleeDamage) + 
                                " (максимум " + std::to_string(MAX_DAMAGE) + ")");
    }
    
    if (rangedDamage < 0) {
        throw std::runtime_error("Некорректный урон дальнего боя: не может быть отрицательным, получено " + 
                                std::to_string(rangedDamage));
    }
    if (rangedDamage > MAX_DAMAGE) {
        throw std::runtime_error("Слишком большой урон дальнего боя: " + std::to_string(rangedDamage) + 
                                " (максимум " + std::to_string(MAX_DAMAGE) + ")");
    }
    
    int combatModeValue;
    try {
        combatModeValue = json["combatMode"].get<int>();
    } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Ошибка чтения режима боя: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw std::runtime_error("Переполнение при чтении режима боя: " + std::string(e.what()));
    }
    
    if (combatModeValue < 0 || combatModeValue > 1) {
        throw std::runtime_error("Некорректный режим боя: " + std::to_string(combatModeValue) + 
                                " (допустимые значения: 0 или 1)");
    }
    
    bool canMove;
    int money;
    try {
        canMove = json["canMove"].get<bool>();
        money = json["money"].get<int>();
    } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Ошибка чтения canMove/money: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw std::runtime_error("Переполнение при чтении canMove/money: " + std::string(e.what()));
    }
    
    const int MAX_MONEY = 1000000000;
    if (money < 0) {
        throw std::runtime_error("Некорректное количество денег: не может быть отрицательным, получено " + 
                                std::to_string(money));
    }
    if (money > MAX_MONEY) {
        throw std::runtime_error("Слишком большое количество денег: " + std::to_string(money) + 
                                " (максимум " + std::to_string(MAX_MONEY) + ")");
    }
    
    player.setMeleeDamage(meleeDamage);
    player.setRangedDamage(rangedDamage);
    player.setCombatMode(static_cast<CombatMode>(combatModeValue));
    player.setCanMove(canMove);
    player.setMoney(money);
    handSerializer.deserialize(player.getHand(), json["hand"]);
}

