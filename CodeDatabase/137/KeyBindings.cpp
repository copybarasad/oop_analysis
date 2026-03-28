#include "KeyBindings.h"
#include "GameException.h"
#include <fstream>
#include <sstream>
#include <algorithm>

KeyBindings::KeyBindings() {
    SetDefaults();
}

void KeyBindings::SetDefaults() {
    key_to_action_.clear();
    action_to_key_.clear();

    key_to_action_[sf::Keyboard::Key::W] = PlayerAction::MOVE_UP;
    key_to_action_[sf::Keyboard::Key::S] = PlayerAction::MOVE_DOWN;
    key_to_action_[sf::Keyboard::Key::A] = PlayerAction::MOVE_LEFT;
    key_to_action_[sf::Keyboard::Key::D] = PlayerAction::MOVE_RIGHT;
    key_to_action_[sf::Keyboard::Key::C] = PlayerAction::SWITCH_COMBAT_MODE;
    key_to_action_[sf::Keyboard::Key::Space] = PlayerAction::SKIP_TURN;
    key_to_action_[sf::Keyboard::Key::E] = PlayerAction::ATTACK;
    key_to_action_[sf::Keyboard::Key::Escape] = PlayerAction::PAUSE_GAME;
    key_to_action_[sf::Keyboard::Key::Enter] = PlayerAction::START_NEW_GAME;
    key_to_action_[sf::Keyboard::Key::L] = PlayerAction::LOAD_GAME;
    key_to_action_[sf::Keyboard::Key::Num1] = PlayerAction::SELECT_UPGRADE_1;
    key_to_action_[sf::Keyboard::Key::Num2] = PlayerAction::SELECT_UPGRADE_2;
    key_to_action_[sf::Keyboard::Key::Num3] = PlayerAction::SELECT_UPGRADE_3;
    key_to_action_[sf::Keyboard::Key::Q] = PlayerAction::QUIT_GAME;

    for (const auto& pair : key_to_action_) {
        action_to_key_[pair.second] = pair.first;
    }
}

void KeyBindings::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        SetDefaults();
        return;
    }

    std::map<sf::Keyboard::Key, PlayerAction> temp_bindings;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string action_str, key_str;
        
        if (std::getline(iss, action_str, '=') && std::getline(iss, key_str)) {
            // Trim whitespace
            action_str.erase(0, action_str.find_first_not_of(" \t"));
            action_str.erase(action_str.find_last_not_of(" \t") + 1);
            key_str.erase(0, key_str.find_first_not_of(" \t"));
            key_str.erase(key_str.find_last_not_of(" \t") + 1);

            PlayerAction action = StringToAction(action_str);
            sf::Keyboard::Key key = StringToKey(key_str);

            if (action != PlayerAction::NONE && key != sf::Keyboard::Key::Unknown) {
                temp_bindings[key] = action;
            }
        }
    }

    file.close();

    // Validate bindings
    key_to_action_ = temp_bindings;
    action_to_key_.clear();
    for (const auto& pair : key_to_action_) {
        action_to_key_[pair.second] = pair.first;
    }

    if (!ValidateBindings()) {
        SetDefaults();
    }
}

void KeyBindings::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileOpenException(filename);
    }

    file << "# Key Bindings Configuration\n";
    file << "# Format: ACTION = KEY\n\n";

    for (const auto& pair : action_to_key_) {
        file << ActionToString(pair.first) << " = " << KeyToString(pair.second) << "\n";
    }

    file.close();
}

PlayerAction KeyBindings::GetAction(sf::Keyboard::Key key) const {
    auto it = key_to_action_.find(key);
    if (it != key_to_action_.end()) {
        return it->second;
    }
    return PlayerAction::NONE;
}

sf::Keyboard::Key KeyBindings::GetKey(PlayerAction action) const {
    auto it = action_to_key_.find(action);
    if (it != action_to_key_.end()) {
        return it->second;
    }
    return sf::Keyboard::Key::Unknown;
}

bool KeyBindings::SetBinding(sf::Keyboard::Key key, PlayerAction action) {
    auto it = key_to_action_.find(key);
    if (it != key_to_action_.end() && it->second != action) {
        return false;
    }

    auto action_it = action_to_key_.find(action);
    if (action_it != action_to_key_.end()) {
        key_to_action_.erase(action_it->second);
    }

    key_to_action_[key] = action;
    action_to_key_[action] = key;
    return true;
}

bool KeyBindings::IsValid() const {
    return ValidateBindings();
}

bool KeyBindings::ValidateBindings() const {
    std::map<PlayerAction, int> action_count;
    for (const auto& pair : key_to_action_) {
        action_count[pair.second]++;
        if (action_count[pair.second] > 1) {
            return false;
        }
    }

    std::vector<PlayerAction> essential_actions = {
        PlayerAction::MOVE_UP, PlayerAction::MOVE_DOWN,
        PlayerAction::MOVE_LEFT, PlayerAction::MOVE_RIGHT,
        PlayerAction::ATTACK, PlayerAction::SKIP_TURN
    };

    for (PlayerAction action : essential_actions) {
        if (action_to_key_.find(action) == action_to_key_.end()) {
            return false;
        }
    }

    return true;
}

sf::Keyboard::Key KeyBindings::StringToKey(const std::string& str) const {
    if (str == "W") return sf::Keyboard::Key::W;
    if (str == "A") return sf::Keyboard::Key::A;
    if (str == "S") return sf::Keyboard::Key::S;
    if (str == "D") return sf::Keyboard::Key::D;
    if (str == "E") return sf::Keyboard::Key::E;
    if (str == "Q") return sf::Keyboard::Key::Q;
    if (str == "C") return sf::Keyboard::Key::C;
    if (str == "L") return sf::Keyboard::Key::L;
    if (str == "R") return sf::Keyboard::Key::R;
    if (str == "Space") return sf::Keyboard::Key::Space;
    if (str == "Enter") return sf::Keyboard::Key::Enter;
    if (str == "Escape") return sf::Keyboard::Key::Escape;
    if (str == "1" || str == "Num1") return sf::Keyboard::Key::Num1;
    if (str == "2" || str == "Num2") return sf::Keyboard::Key::Num2;
    if (str == "3" || str == "Num3") return sf::Keyboard::Key::Num3;
    return sf::Keyboard::Key::Unknown;
}

std::string KeyBindings::KeyToString(sf::Keyboard::Key key) const {
    switch (key) {
        case sf::Keyboard::Key::W: return "W";
        case sf::Keyboard::Key::A: return "A";
        case sf::Keyboard::Key::S: return "S";
        case sf::Keyboard::Key::D: return "D";
        case sf::Keyboard::Key::E: return "E";
        case sf::Keyboard::Key::Q: return "Q";
        case sf::Keyboard::Key::C: return "C";
        case sf::Keyboard::Key::L: return "L";
        case sf::Keyboard::Key::R: return "R";
        case sf::Keyboard::Key::Space: return "Space";
        case sf::Keyboard::Key::Enter: return "Enter";
        case sf::Keyboard::Key::Escape: return "Escape";
        case sf::Keyboard::Key::Num1: return "Num1";
        case sf::Keyboard::Key::Num2: return "Num2";
        case sf::Keyboard::Key::Num3: return "Num3";
        default: return "Unknown";
    }
}

PlayerAction KeyBindings::StringToAction(const std::string& str) const {
    if (str == "MOVE_UP") return PlayerAction::MOVE_UP;
    if (str == "MOVE_DOWN") return PlayerAction::MOVE_DOWN;
    if (str == "MOVE_LEFT") return PlayerAction::MOVE_LEFT;
    if (str == "MOVE_RIGHT") return PlayerAction::MOVE_RIGHT;
    if (str == "SWITCH_COMBAT_MODE") return PlayerAction::SWITCH_COMBAT_MODE;
    if (str == "SKIP_TURN") return PlayerAction::SKIP_TURN;
    if (str == "ATTACK") return PlayerAction::ATTACK;
    if (str == "PAUSE_GAME") return PlayerAction::PAUSE_GAME;
    if (str == "RESUME_GAME") return PlayerAction::RESUME_GAME;
    if (str == "START_NEW_GAME") return PlayerAction::START_NEW_GAME;
    if (str == "LOAD_GAME") return PlayerAction::LOAD_GAME;
    if (str == "SAVE_GAME") return PlayerAction::SAVE_GAME;
    if (str == "QUIT_GAME") return PlayerAction::QUIT_GAME;
    if (str == "SELECT_UPGRADE_1") return PlayerAction::SELECT_UPGRADE_1;
    if (str == "SELECT_UPGRADE_2") return PlayerAction::SELECT_UPGRADE_2;
    if (str == "SELECT_UPGRADE_3") return PlayerAction::SELECT_UPGRADE_3;
    return PlayerAction::NONE;
}

std::string KeyBindings::ActionToString(PlayerAction action) const {
    switch (action) {
        case PlayerAction::MOVE_UP: return "MOVE_UP";
        case PlayerAction::MOVE_DOWN: return "MOVE_DOWN";
        case PlayerAction::MOVE_LEFT: return "MOVE_LEFT";
        case PlayerAction::MOVE_RIGHT: return "MOVE_RIGHT";
        case PlayerAction::SWITCH_COMBAT_MODE: return "SWITCH_COMBAT_MODE";
        case PlayerAction::SKIP_TURN: return "SKIP_TURN";
        case PlayerAction::ATTACK: return "ATTACK";
        case PlayerAction::PAUSE_GAME: return "PAUSE_GAME";
        case PlayerAction::RESUME_GAME: return "RESUME_GAME";
        case PlayerAction::START_NEW_GAME: return "START_NEW_GAME";
        case PlayerAction::LOAD_GAME: return "LOAD_GAME";
        case PlayerAction::SAVE_GAME: return "SAVE_GAME";
        case PlayerAction::QUIT_GAME: return "QUIT_GAME";
        case PlayerAction::SELECT_UPGRADE_1: return "SELECT_UPGRADE_1";
        case PlayerAction::SELECT_UPGRADE_2: return "SELECT_UPGRADE_2";
        case PlayerAction::SELECT_UPGRADE_3: return "SELECT_UPGRADE_3";
        default: return "NONE";
    }
}
