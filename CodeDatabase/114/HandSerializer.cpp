#include "HandSerializer.h"
#include "../Hand.h"

nlohmann::json HandSerializer::serialize(const Hand& hand) const {
    nlohmann::json json;
    json["maxSpellsCount"] = hand.getMaxSpellsCount();
    json["pendingBoostMultiplier"] = hand.getPendingBoostMultiplier();
    json["spellCastDelayActive"] = hand.getSpellCastDelayActive();
    nlohmann::json spellsArray = nlohmann::json::array();
    for (const auto& spell : hand.getSpells()) {
        spellsArray.push_back(static_cast<int>(spell));
    }
    json["spells"] = spellsArray;
    return json;
}

void HandSerializer::deserialize(Hand& hand, const nlohmann::json& json) const {
    if (!json.contains("maxSpellsCount") || !json.contains("pendingBoostMultiplier") ||
        !json.contains("spellCastDelayActive") || !json.contains("spells")) {
        throw std::runtime_error("Некорректный формат JSON для Hand: отсутствуют обязательные поля");
    }
    
    // Проверка типов данных
    if (!json["maxSpellsCount"].is_number_unsigned() || !json["pendingBoostMultiplier"].is_number_integer() ||
        !json["spellCastDelayActive"].is_boolean() || !json["spells"].is_array()) {
        throw std::runtime_error("Некорректный тип данных в JSON для Hand");
    }
    
    std::size_t maxSpellsCount;
    int pendingBoostMultiplier;
    bool spellCastDelayActive;
    try {
        maxSpellsCount = json["maxSpellsCount"].get<std::size_t>();
        pendingBoostMultiplier = json["pendingBoostMultiplier"].get<int>();
        spellCastDelayActive = json["spellCastDelayActive"].get<bool>();
    } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Ошибка чтения значений Hand: " + std::string(e.what()));
    } catch (const nlohmann::json::out_of_range& e) {
        throw std::runtime_error("Переполнение при чтении значений Hand: " + std::string(e.what()));
    }
    
    // Валидация maxSpellsCount
    const std::size_t MAX_SPELLS_COUNT = 100;
    if (maxSpellsCount == 0 || maxSpellsCount > MAX_SPELLS_COUNT) {
        throw std::runtime_error("Некорректное максимальное количество заклинаний: " + 
                                std::to_string(maxSpellsCount) + " (допустимый диапазон: 1-" + 
                                std::to_string(MAX_SPELLS_COUNT) + ")");
    }
    
    // Валидация pendingBoostMultiplier
    const int MAX_BOOST = 1000;
    if (pendingBoostMultiplier < 1 || pendingBoostMultiplier > MAX_BOOST) {
        throw std::runtime_error("Некорректный множитель улучшения: " + 
                                std::to_string(pendingBoostMultiplier) + " (допустимый диапазон: 1-" + 
                                std::to_string(MAX_BOOST) + ")");
    }
    
    hand.setMaxSpellsCount(maxSpellsCount);
    hand.setPendingBoostMultiplier(pendingBoostMultiplier);
    hand.setSpellCastDelayActive(spellCastDelayActive);
    
    // Очищаем текущие заклинания и загружаем новые
    while (hand.getSpells().size() > 0) {
        hand.removeSpell(0);
    }
    
    for (const auto& spellJson : json["spells"]) {
        if (!spellJson.is_number_integer()) {
            throw std::runtime_error("Некорректный тип данных для заклинания в массиве spells");
        }
        
        int spellValue;
        try {
            spellValue = spellJson.get<int>();
        } catch (const nlohmann::json::type_error& e) {
            throw std::runtime_error("Ошибка чтения типа заклинания: " + std::string(e.what()));
        } catch (const nlohmann::json::out_of_range& e) {
            throw std::runtime_error("Переполнение при чтении типа заклинания: " + std::string(e.what()));
        }
        
        if (spellValue < 0 || spellValue > 4) {
            throw std::runtime_error("Некорректный тип заклинания: " + std::to_string(spellValue) + 
                                    " (допустимые значения: 0-4)");
        }
        hand.addSpell(static_cast<SpellType>(spellValue));
    }
    
    // Проверка, что количество заклинаний не превышает максимум
    if (hand.getSpells().size() > maxSpellsCount) {
        throw std::runtime_error("Количество заклинаний (" + std::to_string(hand.getSpells().size()) + 
                                ") превышает максимальное (" + std::to_string(maxSpellsCount) + ")");
    }
}

