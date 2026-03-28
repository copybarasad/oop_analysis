#include "ConsoleInputReader.h"
#include "Commands/MoveCommand.h"
#include "Commands/CastSpellCommand.h"
#include "Commands/BuySpellCommand.h"
#include "Commands/SkipTurnCommand.h"
#include "Commands/SaveCommand.h"
#include "Commands/LoadCommand.h"
#include "Commands/QuitCommand.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>

ConsoleInputReader::ConsoleInputReader() {
    // Load defaults initial
    LoadConfig(""); 
}

void ConsoleInputReader::LoadConfig(const std::string& filename) {
    std::map<std::string, CommandType> tempMap;
    std::set<std::string> mappedCommands; // To check if all commands are mapped
    
    // Helper to map string name to enum
    auto getType = [](const std::string& s) -> CommandType {
        if (s == "move") return CommandType::Move;
        if (s == "cast") return CommandType::Cast;
        if (s == "buy")  return CommandType::Buy;
        if (s == "skip") return CommandType::Skip;
        if (s == "save") return CommandType::Save;
        if (s == "load") return CommandType::Load;
        if (s == "quit") return CommandType::Quit;
        return CommandType::Unknown;
    };

    bool valid = true;
    std::ifstream in(filename);

    if (filename.empty() || !in.is_open()) {
        if (!filename.empty()) std::cerr << "Config file not found. Using defaults.\n";
        valid = false;
    } else {
        std::string cmdName, key;
        while (in >> cmdName >> key) {
            CommandType type = getType(cmdName);
            if (type == CommandType::Unknown) {
                valid = false; break;
            }
            if (tempMap.count(key)) {
                valid = false; break; // Duplicate key
            }
            tempMap[key] = type;
            mappedCommands.insert(cmdName);
        }
        
        // All required commands are mapped
        if (mappedCommands.size() < 7) valid = false;
    }

    if (valid) {
        keyMap_ = tempMap;
    } else {
        // Defaults
        keyMap_.clear();
        keyMap_["m"] = CommandType::Move;
        keyMap_["c"] = CommandType::Cast;
        keyMap_["b"] = CommandType::Buy;
        keyMap_["s"] = CommandType::Skip;
        keyMap_["save"] = CommandType::Save;
        keyMap_["load"] = CommandType::Load;
        keyMap_["q"] = CommandType::Quit;
    }
}

std::string ConsoleInputReader::GetKeyFor(CommandType type) const {
    for (const auto& pair : keyMap_) {
        if (pair.second == type) return pair.first;
    }
    return "???";
}

void ConsoleInputReader::PrintHelp() const {
    std::cout << "Controls:\n";
    std::cout << " " << GetKeyFor(CommandType::Move) << " dx dy           - move by (dx,dy)\n";
    std::cout << " " << GetKeyFor(CommandType::Cast) << " idx x y         - cast spell\n";
    std::cout << " " << GetKeyFor(CommandType::Buy) << "                 - buy spell\n";
    std::cout << " " << GetKeyFor(CommandType::Skip) << "                 - skip turn\n";
    std::cout << " " << GetKeyFor(CommandType::Save) << " <file>       - save game\n";
    std::cout << " " << GetKeyFor(CommandType::Load) << " <file>       - load game\n";
    std::cout << " " << GetKeyFor(CommandType::Quit) << "                 - quit\n";
}

std::unique_ptr<Command> ConsoleInputReader::ReadCommand() {
    std::cout << "Command: ";
    std::string line;
    
    if (!std::getline(std::cin, line)) {
        return std::make_unique<QuitCommand>();
    }

    if (line.empty()) return nullptr;

    std::istringstream iss(line);
    std::string cmdStr;
    iss >> cmdStr;

    auto it = keyMap_.find(cmdStr);
    if (it == keyMap_.end()) {
        std::cout << "Unknown command.\n";
        PrintHelp();
        return nullptr;
    }

    CommandType type = it->second;

    switch (type) {
        case CommandType::Move: {
            int dx, dy;
            if (iss >> dx >> dy) {
                return std::make_unique<MoveCommand>(dx, dy);
            } else {
                std::cout << "Invalid arguments for move.\n";
            }
            break;
        }
        case CommandType::Cast: {
            int idx, x, y;
            if (iss >> idx >> x >> y) {
                return std::make_unique<CastSpellCommand>(idx - 1, x, y);
            } else {
                std::cout << "Invalid arguments for cast.\n";
            }
            break;
        }
        case CommandType::Buy:
            return std::make_unique<BuySpellCommand>();
        case CommandType::Skip:
            return std::make_unique<SkipTurnCommand>();
        case CommandType::Save: {
            std::string filename;
            if (iss >> filename) {
                return std::make_unique<SaveCommand>(filename);
            } else {
                std::cout << "Filename required.\n";
            }
            break;
        }
        case CommandType::Load: {
            std::string filename;
            if (iss >> filename) {
                return std::make_unique<LoadCommand>(filename);
            } else {
                std::cout << "Filename required.\n";
            }
            break;
        }
        case CommandType::Quit:
            return std::make_unique<QuitCommand>();
        default:
            break;
    }

    return nullptr;
}

int ConsoleInputReader::ReadInt() {
    int val;
    while (!(std::cin >> val)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input. Please enter a number: ";
    }
    std::cin.ignore(10000, '\n');
    return val;
}

char ConsoleInputReader::ReadChar() {
    char c;
    std::cin >> c;
    std::cin.ignore(10000, '\n');
    return c;
}

std::string ConsoleInputReader::ReadString() {
    std::string s;
    std::cin >> s;
    std::cin.ignore(10000, '\n');
    return s;
}
