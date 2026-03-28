#include "SettingsSerializer.h"
#include "../Settings.h"
#include <cctype>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace {
bool isSingleCharString(const nlohmann::json& value) {
    return value.is_string() && value.get<std::string>().size() == 1;
}

char extractKey(const nlohmann::json& json, const std::string& fieldName) {
    if (!json.contains(fieldName) || !isSingleCharString(json[fieldName])) {
        throw std::runtime_error("Недопустимое значение для поля: " + fieldName);
    }
    return json[fieldName].get<std::string>()[0];
}
}

nlohmann::json SettingsSerializer::serialize(const Settings& settings) const {
    return {
        {"moveUp", std::string(1, settings.moveUp)},
        {"moveDown", std::string(1, settings.moveDown)},
        {"moveLeft", std::string(1, settings.moveLeft)},
        {"moveRight", std::string(1, settings.moveRight)},
        {"switchCombatMode", std::string(1, settings.switchCombatMode)},
        {"attack", std::string(1, settings.attack)},
        {"castSpell", std::string(1, settings.castSpell)},
        {"buySpell", std::string(1, settings.buySpell)},
        {"saveGame", std::string(1, settings.saveGame)},
        {"loadGame", std::string(1, settings.loadGame)},
        {"help", std::string(1, settings.help)},
        {"exitGame", std::string(1, settings.exitGame)},
        {"startNewGame", std::string(1, settings.startNewGame)}
    };
}

void SettingsSerializer::deserialize(Settings& settings, const nlohmann::json& json) const {
    if (!json.is_object()) {
        throw std::runtime_error("JSON настроек должен быть объектом");
    }

    settings.moveUp = extractKey(json, "moveUp");
    settings.moveDown = extractKey(json, "moveDown");
    settings.moveLeft = extractKey(json, "moveLeft");
    settings.moveRight = extractKey(json, "moveRight");
    settings.switchCombatMode = extractKey(json, "switchCombatMode");
    settings.attack = extractKey(json, "attack");
    settings.castSpell = extractKey(json, "castSpell");
    settings.buySpell = extractKey(json, "buySpell");
    settings.saveGame = extractKey(json, "saveGame");
    settings.loadGame = extractKey(json, "loadGame");
    settings.help = extractKey(json, "help");
    settings.exitGame = extractKey(json, "exitGame");
    settings.startNewGame = extractKey(json, "startNewGame");

    if (!settings.isValid()) {
        throw std::runtime_error("Конфликт клавиш в настройках");
    }
}

