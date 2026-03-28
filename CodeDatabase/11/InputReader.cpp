#include "InputReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <limits>

InputReader::InputReader() {
    setDefaultConfig();
}

void InputReader::setDefaultConfig() {
    keyMap.clear();
    keyMap['w'] = ActionType::MOVE_UP;
    keyMap['s'] = ActionType::MOVE_DOWN;
    keyMap['a'] = ActionType::MOVE_LEFT;
    keyMap['d'] = ActionType::MOVE_RIGHT;
    keyMap['c'] = ActionType::SWITCH_MODE;
    keyMap['e'] = ActionType::CAST_SPELL;
    keyMap['b'] = ActionType::BUY_SPELL;
    keyMap['k'] = ActionType::SAVE;
    keyMap['l'] = ActionType::LOAD;
    keyMap['q'] = ActionType::EXIT;
}

void InputReader::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Warning] Config file '" << filename << "' not found. Using defaults.\n";
        return;
    }

    std::map<std::string, ActionType> strToAction = {
        {"UP", ActionType::MOVE_UP}, {"DOWN", ActionType::MOVE_DOWN},
        {"LEFT", ActionType::MOVE_LEFT}, {"RIGHT", ActionType::MOVE_RIGHT},
        {"SWITCH", ActionType::SWITCH_MODE}, {"CAST", ActionType::CAST_SPELL},
        {"BUY", ActionType::BUY_SPELL}, {"SAVE", ActionType::SAVE},
        {"LOAD", ActionType::LOAD}, {"EXIT", ActionType::EXIT}
    };

    std::map<char, ActionType> tempMap;
    std::set<char> usedKeys;
    std::set<ActionType> usedActions;
    
    std::string line;
    bool errorFound = false;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        char key;
        std::string actionStr;
        
        if (ss >> key >> actionStr) {
            auto it = strToAction.find(actionStr);
            if (it != strToAction.end()) {
                ActionType action = it->second;
                if (usedKeys.count(key)) {
                    std::cerr << "[Error] Key '" << key << "' is used twice. Reverting to defaults.\n";
                    errorFound = true;
                    break;
                }
                if (usedActions.count(action)) {
                    std::cerr << "[Error] Action '" << actionStr << "' is used to multiple keys. Reverting to defaults.\n";
                    errorFound = true;
                    break;
                }

                tempMap[key] = action;
                usedKeys.insert(key);
                usedActions.insert(action);
            }
        }
    }
    
    if (errorFound || tempMap.empty()) {
        if (tempMap.empty() && !errorFound) std::cerr << "[Error] Config is empty. Using defaults.\n";
        std::cerr << "Press ENTER to continue with default controls..." << std::endl;
        std::cin.get();
        setDefaultConfig();
    } else {
        keyMap = tempMap;
    }
}

std::unique_ptr<ICommand> InputReader::readCommand() {
    std::cout << "> ";
    char input;
    if (std::cin >> input) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        auto it = keyMap.find(input);
        if (it != keyMap.end()) {
            return createCommand(it->second);
        }
        std::cout << "Unknown command.\n";
    } else {
        std::cin.clear();
    }
    return nullptr;
}

std::unique_ptr<ICommand> InputReader::createCommand(ActionType action) {
    switch (action) {
        case ActionType::MOVE_UP:    return std::make_unique<MoveCommand>(0, -1);
        case ActionType::MOVE_DOWN:  return std::make_unique<MoveCommand>(0, 1);
        case ActionType::MOVE_LEFT:  return std::make_unique<MoveCommand>(-1, 0);
        case ActionType::MOVE_RIGHT: return std::make_unique<MoveCommand>(1, 0);
        case ActionType::SWITCH_MODE:return std::make_unique<SwitchModeCommand>();
        case ActionType::CAST_SPELL: return std::make_unique<CastSpellCommand>();
        case ActionType::BUY_SPELL:  return std::make_unique<BuySpellCommand>();
        case ActionType::SAVE:       return std::make_unique<SaveCommand>();
        case ActionType::LOAD:       return std::make_unique<LoadCommand>();
        case ActionType::EXIT:       return std::make_unique<ExitCommand>();
        default: return nullptr;
    }
}