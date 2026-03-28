//
// Created by Artem on 26.12.2025.
//

#include <unordered_set>
#include "InputKeysModel.h"

InputKeysModel *InputKeysModel::deserialize(std::map<std::string, std::string> &json) {
    char saveGameKey;
    char confirmActionKey;
    char dismissActionKey;
    char attackKey;
    char spellKey;
    char skipKey;
    char continueKey;

    saveGameKey = getCharValueFromJson(json, "saveGameKey", DEFAULT_SAVE_GAME_KEY);
    confirmActionKey = getCharValueFromJson(json, "confirmActionKey", DEFAULT_CONFIRM_ACTION_KEY);
    dismissActionKey = getCharValueFromJson(json, "dismissActionKey", DEFAULT_DISMISS_ACTION_KEY);
    attackKey = getCharValueFromJson(json, "attackKey", DEFAULT_ATTACK_KEY);
    spellKey = getCharValueFromJson(json, "spellKey", DEFAULT_SPELL_KEY);
    skipKey = getCharValueFromJson(json, "skipKey", DEFAULT_SKIP_KEY);
    continueKey = getCharValueFromJson(json, "continueKey", DEFAULT_CONTINUE_KEY);

    std::vector<char> values = {
            saveGameKey, confirmActionKey, dismissActionKey,
            attackKey, spellKey, skipKey, continueKey
    };

    if (hasDuplicateValues(values)) {
        return new InputKeysModel(
                DEFAULT_SAVE_GAME_KEY,
                DEFAULT_CONFIRM_ACTION_KEY,
                DEFAULT_DISMISS_ACTION_KEY,
                DEFAULT_ATTACK_KEY,
                DEFAULT_SPELL_KEY,
                DEFAULT_SKIP_KEY,
                DEFAULT_CONTINUE_KEY
        );
    }

    return new InputKeysModel(
            saveGameKey, confirmActionKey, dismissActionKey,
            attackKey, spellKey, skipKey, continueKey
    );
}

std::string InputKeysModel::serialize() {
    std::string res = "{";
    res += "saveGameKey:" + std::to_string(static_cast<int>(this->saveGameKey)) + ",";
    res += "confirmActionKey:" + std::to_string(static_cast<int>(this->confirmActionKey)) + ",";
    res += "dismissActionKey:" + std::to_string(static_cast<int>(this->dismissActionKey)) + ",";
    res += "attackKey:" + std::to_string(static_cast<int>(this->attackKey)) + ",";
    res += "spellKey:" + std::to_string(static_cast<int>(this->spellKey)) + ",";
    res += "skipKey:" + std::to_string(static_cast<int>(this->skipKey)) + ",";
    res += "continueKey:" + std::to_string(static_cast<int>(this->continueKey));
    res += "}";
    return res;
}


char InputKeysModel::getCharValueFromJson(const std::map<std::string, std::string> &json, const std::string &key,
                                          char defaultValue) {
    auto it = json.find(key);
    if (it != json.end()) {
        try {
            return static_cast<char>(std::stoi(it->second));
        } catch (const std::exception& e) {
            return defaultValue;
        }
    }
    return defaultValue;
}

bool InputKeysModel::hasDuplicateValues(const std::vector<char> &values) {
    std::unordered_set<char> uniqueValues;
    for (char value : values) {
        if (uniqueValues.count(value) > 0) {
            return true;
        }
        uniqueValues.insert(value);
    }
    return false;
}
