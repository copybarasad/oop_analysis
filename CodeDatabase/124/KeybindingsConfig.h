#ifndef KEYBINDINGS_CONFIG_H
#define KEYBINDINGS_CONFIG_H

#include <unordered_map>
#include <string>
#include <vector>

struct CommandConfig {
    std::string description;
    bool consumesTurn;
};

class KeybindingsConfig {
private:
    std::unordered_map<char, std::string> keyToCommand;
    std::unordered_map<std::string, CommandConfig> commandConfigs;
    std::string configFile = "keybindings.json";

public:
    KeybindingsConfig();
    
    bool loadFromFile();
    bool saveToFile() const;
    void setDefaultKeybindings();
    
    std::string getCommand(char key) const;
    CommandConfig getCommandConfig(const std::string& command) const;
    std::vector<char> getKeysForCommand(const std::string& command) const;
    
    bool validateConfig() const;
    void printCurrentBindings() const;
};

#endif