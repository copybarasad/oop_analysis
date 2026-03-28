#include "input_config.h"
#include <fstream>
#include <iostream>

InputConfig::InputConfig() {
    setDefaults();
}

void InputConfig::setDefaults() {
    key_map.clear();
    
    key_map['w'] = {CommandType::Move, "Move Up", 0, -1};
    key_map['s'] = {CommandType::Move, "Move Down", 0, 1};
    key_map['a'] = {CommandType::Move, "Move Left", -1, 0};
    key_map['d'] = {CommandType::Move, "Move Right", 1, 0};
    
    key_map['f'] = {CommandType::Attack, "Attack"};
    key_map['r'] = {CommandType::SwitchWeapon, "Switch Weapon"};
    key_map['c'] = {CommandType::CastSpell, "Cast Spell"};
    key_map['b'] = {CommandType::OpenStore, "Open Store"};
    
    key_map['z'] = {CommandType::Save, "Save Game"};
    key_map['l'] = {CommandType::Load, "Load Game"}; 
    key_map['q'] = {CommandType::Quit, "Quit Game"};
}

void InputConfig::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        setDefaults();  
        return;
    }

    std::map<std::string, Command> registry;
    registry["move_up"]    = {CommandType::Move, "Move Up", 0, -1};
    registry["move_down"]  = {CommandType::Move, "Move Down", 0, 1};
    registry["move_left"]  = {CommandType::Move, "Move Left", -1, 0};
    registry["move_right"] = {CommandType::Move, "Move Right", 1, 0};
    registry["attack"]     = {CommandType::Attack, "Attack"};
    registry["switch"]     = {CommandType::SwitchWeapon, "Switch"};
    registry["cast"]       = {CommandType::CastSpell, "Cast"};
    registry["store"]      = {CommandType::OpenStore, "Store"};
    registry["save"]       = {CommandType::Save, "Save"};
    registry["load"]       = {CommandType::Load, "Load"};
    registry["quit"]       = {CommandType::Quit, "Quit"};

    std::map<char, std::string> key_to_action;   
    std::map<std::string, char> action_to_key; 
    std::map<char, Command> temp_map;
    
    char key;
    std::string action;
    bool has_error = false;

    while (file >> key >> action) {
        if (registry.find(action) == registry.end()) {
            has_error = true;  
            break;
        }
        
        if (key_to_action.find(key) != key_to_action.end()) {
            if (key_to_action[key] != action) {  
                has_error = true;  
                break;
            }
            continue;
        }
        
        if (action_to_key.find(action) != action_to_key.end()) {
            has_error = true;  
            break;
        }
        
        key_to_action[key] = action;
        action_to_key[action] = key;
        temp_map[key] = registry[action];
    }

    std::vector<std::string> required_actions;
    required_actions.push_back("move_up");
    required_actions.push_back("move_down");
    required_actions.push_back("move_left");
    required_actions.push_back("move_right");
    required_actions.push_back("attack");
    
    for (size_t i = 0; i < required_actions.size(); ++i) {
        const std::string& req_action = required_actions[i];
        if (action_to_key.find(req_action) == action_to_key.end()) {
            has_error = true;  
            break;
        }
    }

    if (has_error || temp_map.empty()) {
        std::cout << "Config file has errors. Using default controls." << std::endl;
        setDefaults();  
    } else {
        key_map = temp_map;
        std::cout << "Configuration loaded successfully." << std::endl;
    }
}

Command InputConfig::getCommand(char key) const {
    auto it = key_map.find(key);
    if (it != key_map.end()) {
        return it->second;
    }
    return {CommandType::None, "Unknown"};
}