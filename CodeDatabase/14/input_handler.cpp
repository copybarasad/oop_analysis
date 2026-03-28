#include "input_handler.hpp"
#include "commands.hpp"
#include <iostream>
#include <fstream>

ConsoleInputHandler::ConsoleInputHandler() {
    loadDefaultConfig();
}

void ConsoleInputHandler::loadDefaultConfig() {
    keyBindings.clear();
    keyBindings["w"] = "MOVE_UP";
    keyBindings["s"] = "MOVE_DOWN";
    keyBindings["a"] = "MOVE_LEFT";
    keyBindings["d"] = "MOVE_RIGHT";
    keyBindings["f"] = "ATTACK";
    keyBindings["e"] = "SWITCH_WEAPON";
    keyBindings["c"] = "CAST_SPELL";
    keyBindings["save"] = "SAVE";
    keyBindings["load"] = "LOAD";
    keyBindings["q"] = "EXIT";
}

void ConsoleInputHandler::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::map<std::string, std::string> tempBindings;
    std::map<std::string, bool> keyUsed;
    std::string line;
    bool error = false;

    while (std::getline(file, line)) {
        size_t delimiter = line.find('=');
        if (delimiter == std::string::npos) continue;

        std::string key = line.substr(0, delimiter);
        std::string action = line.substr(delimiter + 1);

        if (keyUsed[key]) { error = true; break; }
        
        tempBindings[key] = action;
        keyUsed[key] = true;
    }

    if (!error && !tempBindings.empty()) {
        keyBindings = tempBindings;
        std::cout << "Config loaded successfully." << std::endl;
    } else {
        std::cout << "Config error. Using defaults." << std::endl;
        loadDefaultConfig();
    }
}

std::string ConsoleInputHandler::getKey(const std::string& action) const {
    for (const auto& pair : keyBindings) {
        if (pair.second == action) {
            return pair.first; 
        }
    }
    return "???";
}

Command* ConsoleInputHandler::handleInput(Game& game) {
    (void)game;
    std::cout << "> ";
    std::string input;
    std::cin >> input;

    auto it = keyBindings.find(input);
    if (it == keyBindings.end()) return new NullCommand();

    std::string action = it->second;

    if (action == "MOVE_UP") return new MoveCommand(0, -1);
    if (action == "MOVE_DOWN") return new MoveCommand(0, 1);
    if (action == "MOVE_LEFT") return new MoveCommand(-1, 0);
    if (action == "MOVE_RIGHT") return new MoveCommand(1, 0);
    if (action == "ATTACK") return new AttackCommand();
    if (action == "SWITCH_WEAPON") return new SwitchWeaponCommand();
    if (action == "SAVE") return new SaveCommand();
    if (action == "LOAD") return new LoadCommand();
    if (action == "EXIT") return new ExitCommand();
    
    if (action == "CAST_SPELL") {
        int idx;
        std::cout << "Enter spell index: ";
        std::cin >> idx;
        int x = 0, y = 0;
        std::cout << "Enter target X Y (or 0 0): ";
        std::cin >> x >> y;
        return new CastSpellCommand(idx, x, y);
    }

    return new NullCommand();
}