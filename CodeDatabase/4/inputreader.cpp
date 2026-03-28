#include "inputreader.hpp"
#include <fstream>
#include <iostream>
#include <unordered_set>

void InputReader::load_default_mapping() {
    keymap = {
        {'w', GameAction::MOVE_UP},
        {'s', GameAction::MOVE_DOWN},
        {'a', GameAction::MOVE_LEFT},
        {'d', GameAction::MOVE_RIGHT},
        {'m', GameAction::CAST_SPELL},
        {'c', GameAction::SWAP_WEAPON},
        {'b', GameAction::BUY_SPELL},
        {'q', GameAction::QUIT},
        {'S', GameAction::SAVE}
    };
}

bool InputReader::is_valid_mapping() const {
    std::unordered_map<GameAction, int> action_count;
    std::unordered_set<char> used_keys;
    for (const auto& [key, act] : keymap) {
        if (used_keys.count(key)) return false;
        used_keys.insert(key);
        action_count[act]++;
        if (action_count[act] > 1) return false;
    }
    for (int i = 0; i < static_cast<int>(GameAction::INVALID); ++i) {
        if (i == static_cast<int>(GameAction::SAVE)) continue;
        if (action_count[static_cast<GameAction>(i)] == 0) return false;
    }
    return true;
}

InputReader::InputReader(const std::string& config_path) {
    load_default_mapping();
    try {
        std::ifstream file(config_path);
        if (!file.is_open()) return;

        std::unordered_map<GameAction, char> reverse_map;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            size_t pos = line.find('=');
            if (pos == std::string::npos) continue;
            std::string action_str = line.substr(0, pos);
            char key = line[pos + 1];
            GameAction act;
            if (action_str == "MOVE_UP") act = GameAction::MOVE_UP;
            else if (action_str == "MOVE_DOWN") act = GameAction::MOVE_DOWN;
            else if (action_str == "MOVE_LEFT") act = GameAction::MOVE_LEFT;
            else if (action_str == "MOVE_RIGHT") act = GameAction::MOVE_RIGHT;
            else if (action_str == "CAST_SPELL") act = GameAction::CAST_SPELL;
            else if (action_str == "SWAP_WEAPON") act = GameAction::SWAP_WEAPON;
            else if (action_str == "BUY_SPELL") act = GameAction::BUY_SPELL;
            else if (action_str == "QUIT") act = GameAction::QUIT;
            else if (action_str == "SAVE") act = GameAction::SAVE;
            else continue;

            reverse_map[act] = key;
        }

        keymap.clear();
        for (const auto& [act, key] : reverse_map) {
            keymap[key] = act;
        }

        if (!is_valid_mapping()) {
            std::cerr << "Invalid control config. Using defaults.\n";
            load_default_mapping();
        }
    } catch (...) {
        std::cerr << "Failed to read control config. Using defaults.\n";
        load_default_mapping();
    }
}