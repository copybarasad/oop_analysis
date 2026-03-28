#pragma once
#include <unordered_map>
#include <string>
#include <vector>

class ControlConfig {
private:
    std::unordered_map<std::string, std::string> commandToKey;
    std::unordered_map<std::string, std::string> keyToCommand;
    const std::unordered_map<std::string, std::string> defaultConfig = {
        {"UP", "UP"},
        {"DOWN", "DOWN"},
        {"LEFT", "LEFT"}, 
        {"RIGHT", "RIGHT"},
        {"ATTACK", "ATTACK"},
        {"SHOW_SPELLS", "SHOW_SPELLS"},
        {"BUY_SPELL", "BUY_SPELL"},
        {"USE_SPELL", "USE_SPELL"},
        {"CHANGE_TYPE", "CHANGE_TYPE"},
        {"SHOW_STATUS", "SHOW_STATUS"},
        {"SAVE", "SAVE"},
        {"LOAD", "LOAD"},
        {"QUIT", "QUIT"},
        {"SHOW_MENU", "SHOW_MENU"}
    };

public:
    ControlConfig();
    bool loadFromFile(const std::string& filename);
    void setDefaultConfig();
    bool isValid() const;
    std::string getCommandForKey(const std::string& key) const;
    std::string getKeyForCommand(const std::string& command) const;
};