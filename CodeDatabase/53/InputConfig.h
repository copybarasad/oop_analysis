#ifndef INPUTCONFIG_H
#define INPUTCONFIG_H

#include <map>
#include <string>
#include <unordered_set>

class InputConfig {
private:
    std::map<char, std::string> keyToCommand;
    std::map<std::string, char> commandToKey;
    
    void setDefaults();
    bool isValidConfig() const;
    
public:
    InputConfig();
    
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
    std::string getCommandForKey(char key) const;
    char getKeyForCommand(const std::string& command) const;
    std::unordered_set<std::string> getAllCommands() const;
    std::unordered_set<char> getAllKeys() const;
};

#endif