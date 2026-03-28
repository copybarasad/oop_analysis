#include "InputReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

InputReader::InputReader() {
    key_map_['w'] = ActionType::kMoveUp;
    key_map_['s'] = ActionType::kMoveDown;
    key_map_['a'] = ActionType::kMoveLeft;
    key_map_['d'] = ActionType::kMoveRight;
    key_map_['e'] = ActionType::kAction;
    key_map_['f'] = ActionType::kSwitchStance;
    key_map_['c'] = ActionType::kCastMode;
    key_map_['b'] = ActionType::kBuy;
    key_map_['k'] = ActionType::kSave;
    key_map_['l'] = ActionType::kLoad;
    key_map_['q'] = ActionType::kQuit;
    key_map_['x'] = ActionType::kCancel;
}

void InputReader::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::map<char, ActionType> new_map;
    std::map<ActionType, char> reverse_check;
    std::string line;
    
    std::map<std::string, ActionType> str_to_action = {
        {"UP", ActionType::kMoveUp}, {"DOWN", ActionType::kMoveDown},
        {"LEFT", ActionType::kMoveLeft}, {"RIGHT", ActionType::kMoveRight},
        {"ACTION", ActionType::kAction}, {"STANCE", ActionType::kSwitchStance},
        {"CAST", ActionType::kCastMode}, {"BUY", ActionType::kBuy},
        {"SAVE", ActionType::kSave}, {"LOAD", ActionType::kLoad},
        {"QUIT", ActionType::kQuit}, {"CANCEL", ActionType::kCancel}
    };

    try {
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string segment;
            std::vector<std::string> parts;
            while(std::getline(ss, segment, '=')) parts.push_back(segment);
            
            if (parts.size() != 2) continue;
            
            char key = parts[0][0];
            std::string actionStr = parts[1];
            while(!actionStr.empty() && isspace(actionStr.back())) actionStr.pop_back();

            if (str_to_action.find(actionStr) != str_to_action.end()) {
                ActionType act = str_to_action[actionStr];
                if (new_map.count(key) || reverse_check.count(act)) {
                    std::cerr << "Config conflict. Using defaults." << std::endl;
                    return; 
                }
                new_map[key] = act;
                reverse_check[act] = key;
            }
        }
        if (!new_map.empty()) {
            key_map_ = new_map;
            std::cout << "Custom controls loaded." << std::endl;
        }
    } catch (...) {
        std::cerr << "Error reading config. Using defaults." << std::endl;
    }
}

std::unique_ptr<Command> InputReader::readCommand() {
    std::cout << "Input: ";
    char input;
    std::cin >> input;

    if (isdigit(input)) {
        return std::make_unique<SelectSpellCommand>(input - '1');
    }

    auto it = key_map_.find(input);
    if (it != key_map_.end()) {
        return createCommand(it->second);
    }

    return std::make_unique<UnknownCommand>(input);
}

std::unique_ptr<Command> InputReader::createCommand(ActionType action) {
    switch (action) {
        case ActionType::kMoveUp: return std::make_unique<MoveUpCommand>();
        case ActionType::kMoveDown: return std::make_unique<MoveDownCommand>();
        case ActionType::kMoveLeft: return std::make_unique<MoveLeftCommand>();
        case ActionType::kMoveRight: return std::make_unique<MoveRightCommand>();
        case ActionType::kAction: return std::make_unique<AttackCommand>();
        case ActionType::kSwitchStance: return std::make_unique<SwitchStanceCommand>();
        case ActionType::kCastMode: return std::make_unique<EnterCastingCommand>();
        case ActionType::kBuy: return std::make_unique<BuySpellCommand>();
        case ActionType::kSave: return std::make_unique<SaveCommand>();
        case ActionType::kLoad: return std::make_unique<LoadCommand>();
        case ActionType::kQuit: return std::make_unique<QuitCommand>();
        case ActionType::kCancel: return std::make_unique<ExitCastingCommand>();
        default: return std::make_unique<NullCommand>();
    }
}

std::string InputReader::actionToString(ActionType action) const {
    switch (action) {
        case ActionType::kMoveUp: return "Move Up / Cursor Up";
        case ActionType::kMoveDown: return "Move Down / Cursor Down";
        case ActionType::kMoveLeft: return "Move Left / Cursor Left";
        case ActionType::kMoveRight: return "Move Right / Cursor Right";
        case ActionType::kAction: return "Interact / Cast Spell (in mode)";
        case ActionType::kSwitchStance: return "Switch Stance (Melee/Ranged)";
        case ActionType::kCastMode: return "Enter Casting Mode";
        case ActionType::kBuy: return "Buy Spell (Random/Shop)";
        case ActionType::kSave: return "Quick Save";
        case ActionType::kLoad: return "Quick Load";
        case ActionType::kQuit: return "Quit Game";
        case ActionType::kCancel: return "Cancel / Exit Mode";
        default: return "Unknown";
    }
}

std::string InputReader::getControlsDescription() const {
    std::stringstream ss;

    auto findKey = [&](ActionType action) -> char {
        for (const auto& pair : key_map_) {
            if (pair.second == action) return pair.first;
        }
        return 0;
    };

    std::vector<ActionType> orderedActions = {
        ActionType::kMoveUp,
        ActionType::kMoveLeft,
        ActionType::kMoveDown,
        ActionType::kMoveRight,
        
        ActionType::kAction,
        ActionType::kSwitchStance,
        ActionType::kCastMode,
        ActionType::kCancel,
        ActionType::kBuy,
        
        ActionType::kSave,
        ActionType::kLoad,
        ActionType::kQuit
    };

    for (const auto& action : orderedActions) {
        char key = findKey(action);
        if (key != 0) {
            ss << "  [" << key << "] : " << actionToString(action) << "\n";
        }
    }
    
    ss << "  [1-9] : Select Spell (in Casting Mode)\n";
    
    return ss.str();
}