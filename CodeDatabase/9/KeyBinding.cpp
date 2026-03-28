#include "KeyBinding.h"

#include <fstream>
#include <set>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

KeyBinding::KeyBinding() {
    SetDefault();
}

void KeyBinding::LoadFromFile(const std::string& filename) {
    if (!LoadFromJson(filename)) {
        SetDefault();
    }
}

bool KeyBinding::TryGetAction(sf::Keyboard::Key key, InputAction& action) const {
    auto it = key_to_action_.find(key);
    if (it == key_to_action_.end()) {
        return false;
    }
    action = it->second;
    return true;
}

void KeyBinding::SetDefault() {
    key_to_action_.clear();

    key_to_action_[sf::Keyboard::W] = InputAction::MoveUp;
    key_to_action_[sf::Keyboard::S] = InputAction::MoveDown;
    key_to_action_[sf::Keyboard::A] = InputAction::MoveLeft;
    key_to_action_[sf::Keyboard::D] = InputAction::MoveRight;

    key_to_action_[sf::Keyboard::F] = InputAction::ToggleAttackMode;
    key_to_action_[sf::Keyboard::Q] = InputAction::BuySpell;

    key_to_action_[sf::Keyboard::Num1] = InputAction::SelectCard1;
    key_to_action_[sf::Keyboard::Num2] = InputAction::SelectCard2;
    key_to_action_[sf::Keyboard::Num3] = InputAction::SelectCard3;
    key_to_action_[sf::Keyboard::Num4] = InputAction::SelectCard4;
    key_to_action_[sf::Keyboard::Num5] = InputAction::SelectCard5;
    key_to_action_[sf::Keyboard::Num6] = InputAction::SelectCard6;
    key_to_action_[sf::Keyboard::Num7] = InputAction::SelectCard7;
    key_to_action_[sf::Keyboard::Num8] = InputAction::SelectCard8;
    key_to_action_[sf::Keyboard::Num9] = InputAction::SelectCard9;

    key_to_action_[sf::Keyboard::F5] = InputAction::SaveGame;
    key_to_action_[sf::Keyboard::F9] = InputAction::LoadGame;
}

bool KeyBinding::LoadFromJson(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    json root;
    try {
        in >> root;
    } catch (const json::parse_error&) {
        return false;
    }

    if (!root.is_object()) {
        return false;
    }

    std::map<sf::Keyboard::Key, InputAction> temp_key_to_action;
    std::set<InputAction> seen_actions;

    for (auto it = root.begin(); it != root.end(); ++it) {
        const std::string action_name = it.key();

        InputAction action;
        if (!ParseAction(action_name, action)) {
            return false;
        }

        if (!it.value().is_string()) {
            return false;
        }

        std::string key_name = it.value().get<std::string>();

        sf::Keyboard::Key key;
        if (!ParseKey(key_name, key)) {
            return false;
        }

        if (seen_actions.count(action) > 0) {
            return false;
        }

        auto key_it = temp_key_to_action.find(key);
        if (key_it != temp_key_to_action.end() && key_it->second != action) {
            return false;
        }

        temp_key_to_action[key] = action;
        seen_actions.insert(action);
    }

    const InputAction required_actions[] = {
        InputAction::MoveUp,
        InputAction::MoveDown,
        InputAction::MoveLeft,
        InputAction::MoveRight,
        InputAction::ToggleAttackMode,
        InputAction::BuySpell,
        InputAction::SelectCard1,
        InputAction::SelectCard2,
        InputAction::SelectCard3,
        InputAction::SelectCard4,
        InputAction::SelectCard5,
        InputAction::SelectCard6,
        InputAction::SelectCard7,
        InputAction::SelectCard8,
        InputAction::SelectCard9,
        InputAction::SaveGame,
        InputAction::LoadGame
    };

    for (InputAction required : required_actions) {
        if (seen_actions.count(required) == 0) {
            return false;
        }
    }

    key_to_action_ = std::move(temp_key_to_action);
    return true;
}

bool KeyBinding::ParseKey(const std::string& name, sf::Keyboard::Key& key) {
    if (name.size() == 1) {
        char c = name[0];
        if (c >= 'a' && c <= 'z') {
            c = static_cast<char>(c - 'a' + 'A');
        }
        if (c >= 'A' && c <= 'Z') {
            int idx = c - 'A';
            key = static_cast<sf::Keyboard::Key>(sf::Keyboard::A + idx);
            return true;
        }
        if (c >= '0' && c <= '9') {
            int idx = c - '0';
            key = static_cast<sf::Keyboard::Key>(sf::Keyboard::Num0 + idx);
            return true;
        }
    }

    if (name.rfind("Num", 0) == 0 && name.size() == 4) {
        char d = name[3];
        if (d >= '0' && d <= '9') {
            int idx = d - '0';
            key = static_cast<sf::Keyboard::Key>(sf::Keyboard::Num0 + idx);
            return true;
        }
    }

    if (name == "Up") { key = sf::Keyboard::Up;    return true; }
    if (name == "Down") { key = sf::Keyboard::Down;  return true; }
    if (name == "Left") { key = sf::Keyboard::Left;  return true; }
    if (name == "Right") { key = sf::Keyboard::Right; return true; }

    if (name.size() == 2 && name[0] == 'F' && name[1] >= '1' && name[1] <= '9') {
        int idx = name[1] - '1';
        key = static_cast<sf::Keyboard::Key>(sf::Keyboard::F1 + idx);
        return true;
    }
    if (name.size() == 3 && name[0] == 'F' && name[1] == '1' && (name[2] == '0' || name[2] == '1' || name[2] == '2')) {
        int idx = 9 + (name[2] - '0');
        key = static_cast<sf::Keyboard::Key>(sf::Keyboard::F1 + idx);
        return true;
    }

    if (name == "Space")  { key = sf::Keyboard::Space;  return true; }
    if (name == "Escape") { key = sf::Keyboard::Escape; return true; }

    return false;
}

bool KeyBinding::ParseAction(const std::string& name, InputAction& action) {
    if (name == "MoveUp") { action = InputAction::MoveUp; return true; }
    if (name == "MoveDown") { action = InputAction::MoveDown; return true; }
    if (name == "MoveLeft") { action = InputAction::MoveLeft; return true; }
    if (name == "MoveRight") { action = InputAction::MoveRight; return true; }

    if (name == "ToggleAttackMode") { action = InputAction::ToggleAttackMode;  return true; }
    if (name == "BuySpell") { action = InputAction::BuySpell; return true; }

    if (name == "SelectCard1") { action = InputAction::SelectCard1; return true; }
    if (name == "SelectCard2") { action = InputAction::SelectCard2; return true; }
    if (name == "SelectCard3") { action = InputAction::SelectCard3; return true; }
    if (name == "SelectCard4") { action = InputAction::SelectCard4; return true; }
    if (name == "SelectCard5") { action = InputAction::SelectCard5; return true; }
    if (name == "SelectCard6") { action = InputAction::SelectCard6; return true; }
    if (name == "SelectCard7") { action = InputAction::SelectCard7; return true; }
    if (name == "SelectCard8") { action = InputAction::SelectCard8; return true; }
    if (name == "SelectCard9"){ action = InputAction::SelectCard9; return true; }
    if (name == "SaveGame") { action = InputAction::SaveGame; return true; }
    if (name == "LoadGame") { action = InputAction::LoadGame; return true; }


    return false;
}