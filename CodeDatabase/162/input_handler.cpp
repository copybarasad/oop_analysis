#include "input_handler.h"
#include <sstream>
#include <cctype>
#include <set>

std::map<std::string, std::string> InputHandler::GetDefaultControls() const {
    return {
        {"w", "move_up"},
        {"a", "move_left"},
        {"s", "move_down"},
        {"d", "move_right"},
        {"attack", "attack"},
        {"switch_mode", "switch_mode"},
        {"buy_spell", "buy_spell"},
        {"cast", "cast"},
        {"info", "info"},
        {"save", "save"},
        {"load", "load"},
        {"quit", "quit"},
        {"help", "help"}
    };
}

void InputHandler::LoadControls() {
    std::ifstream file("controls.txt");
    if (!file) {
        std::cout << "Controls file not found. Using default controls.\n";
        controls_ = GetDefaultControls();
        return;
    }

    std::map<std::string, std::string> loaded;
    std::string line;
    bool valid = true;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos == std::string::npos || pos == 0 || pos == line.size() - 1) {
            valid = false;
            break;
        }
        std::string user_key = line.substr(0, pos);
        std::string internal_command = line.substr(pos + 1);

        std::transform(user_key.begin(), user_key.end(), user_key.begin(), ::tolower);
        loaded[user_key] = internal_command;
    }

    if (!valid || !ValidateControls(loaded)) {
        std::cout << "Invalid controls file. Using default controls.\n";
        controls_ = GetDefaultControls();
    } else {
        controls_ = loaded;
    }
}

bool InputHandler::ValidateControls(const std::map<std::string, std::string>& controls) const {
    auto def = GetDefaultControls();
    std::set<std::string> required_commands;
    for (const auto& p : def) {
        required_commands.insert(p.second); 
    }

    std::set<std::string> seen_commands;
    std::set<std::string> seen_keys;
    for (const auto& p : controls) {
        const std::string& key = p.first;
        const std::string& cmd = p.second;

        if (seen_keys.count(key) || seen_commands.count(cmd)) {
            return false;
        }
        seen_keys.insert(key);
        seen_commands.insert(cmd);

        required_commands.erase(cmd);
    }

    return required_commands.empty(); 
}

std::vector<std::string> InputHandler::SplitString(const std::string& str, char delimiter) const {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

InputHandler::Command InputHandler::ParseInput(const std::string& input) const {
    if (input.empty()) {
        return Command("unknown");
    }
    
    std::string lower_input = input;
    std::transform(lower_input.begin(), lower_input.end(), lower_input.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    std::vector<std::string> parts = SplitString(lower_input, ' ');
    
    if (parts.empty()) {
        return Command("unknown");
    }
    
    std::string user_command = parts[0];
    std::vector<std::string> args(parts.begin() + 1, parts.end());
    auto it = controls_.find(user_command);
    if (it != controls_.end()) {
        return Command(it->second, args);
    }
    return Command("unknown", args);
}

bool InputHandler::IsMovementCommand(const std::string& input) {
    return input == "w" || input == "a" || input == "s" || input == "d";
}

bool InputHandler::IsAttackCommand(const std::string& input) {
    return input == "attack";
}

bool InputHandler::IsBuildCommand(const std::string& input) {
    return input == "buy_spell";
}

bool InputHandler::IsSpellCommand(const std::string& input) {
    return input == "cast";
}

bool InputHandler::IsModeSwitchCommand(const std::string& input) {
    return input == "switch_mode";
}

bool InputHandler::IsHelpCommand(const std::string& input) {
    return input == "help";
}

bool InputHandler::IsSaveCommand(const std::string& input) {
    return input == "save";
}

bool InputHandler::IsLoadCommand(const std::string& input) {
    return input == "load";
}

bool InputHandler::IsExitCommand(const std::string& input) {
    return input == "quit";
}
