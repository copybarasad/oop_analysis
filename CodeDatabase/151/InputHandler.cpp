#include "InputHandler.h"
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <sstream>

Position ConsoleInputHandler::getTargetFromUser() const {
    int x, y;
    std::cout << "Target X Y: ";
    std::cin >> x >> y;
    return Position(x, y);
}

void ConsoleInputHandler::loadDefaultConfig() {
    keyToCommand.clear();
    commandToKey.clear();

    keyToCommand['w'] = "move_up";
    keyToCommand['s'] = "move_down";
    keyToCommand['a'] = "move_left";
    keyToCommand['d'] = "move_right";
    keyToCommand['1'] = "cast_0";
    keyToCommand['2'] = "cast_1";
    keyToCommand['3'] = "cast_2";
    keyToCommand['4'] = "cast_3";
    keyToCommand['5'] = "cast_4";
    keyToCommand['z'] = "save";
    keyToCommand['c'] = "load";
    keyToCommand['b'] = "buy";
    keyToCommand['h'] = "help";
    keyToCommand['q'] = "quit";

    for (auto& p : keyToCommand)
        commandToKey[p.second] = p.first;
}

bool ConsoleInputHandler::loadConfigFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::map<char, std::string> tempKeyMap;
    std::set<char> usedKeys;
    std::set<std::string> usedCommands;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line); 
        char key;
        std::string cmd;

        if (!(iss >> key >> cmd)) {    
            continue;  
        }

        if (usedKeys.count(key) || usedCommands.count(cmd)) {
            return false;  
        }

        tempKeyMap[key] = cmd;
        usedKeys.insert(key);
        usedCommands.insert(cmd);
    }

    // Проверка обязательных команд
    const std::vector<std::string> required = {
        "move_up", "move_down", "move_left", "move_right",
        "save", "load", "quit"
    };
    for (const auto& r : required) {
        if (usedCommands.find(r) == usedCommands.end())
            return false;
    }

    keyToCommand = std::move(tempKeyMap);
    commandToKey.clear();
    for (const auto& p : keyToCommand)
        commandToKey[p.second] = p.first;

    return true;
}

ConsoleInputHandler::ConsoleInputHandler(const std::string& configFile) {
    if (!loadConfigFromFile(configFile))
        loadDefaultConfig();
}

std::unique_ptr<Command> ConsoleInputHandler::getNextCommand() {
    std::cout << "Command: ";
    char ch;
    if (!(std::cin >> ch)) return nullptr;

    auto it = keyToCommand.find(ch);
    if (it == keyToCommand.end()) {
        std::cout << "Unknown key!\n";
        return nullptr;
    }

    const std::string& cmd = it->second;

    if (cmd == "move_up")    return std::make_unique<MoveCommand>(0, -1);
    if (cmd == "move_down")  return std::make_unique<MoveCommand>(0, 1);
    if (cmd == "move_left")  return std::make_unique<MoveCommand>(-1, 0);
    if (cmd == "move_right") return std::make_unique<MoveCommand>(1, 0);
    if (cmd.substr(0, 5) == "cast_") {
        int idx = std::stoi(cmd.substr(5));
        return std::make_unique<CastSpellCommand>(idx, getTargetFromUser());
    }
    if (cmd == "save") return std::make_unique<SaveCommand>();
    if (cmd == "load") return std::make_unique<LoadCommand>();
    if (cmd == "buy")  return std::make_unique<BuySpellCommand>();
    if (cmd == "help") return std::make_unique<HelpCommand>(keyToCommand);
    if (cmd == "quit") return std::make_unique<QuitCommand>();

    return nullptr;
}