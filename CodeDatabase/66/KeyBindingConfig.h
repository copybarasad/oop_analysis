#ifndef KEYBINDINGCONFIG_H
#define KEYBINDINGCONFIG_H

#include "Command.h"
#include "Direction.h"
#include <map>
#include <string>
#include <vector>
#include <memory>

class KeyBindingException : public std::exception {
private:
    std::string message;
public:
    explicit KeyBindingException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class KeyBindingConfig {
private:
    std::map<char, CommandType> keyToCommand;
    std::map<char, Direction> keyToDirection;
    std::map<CommandType, char> commandToKey;
    bool isLoaded;
    
    void setDefaultBindings();
    void validateBindings() const;
    bool parseCommandType(const std::string& commandStr, CommandType& outType) const;
    bool parseDirection(const std::string& dirStr, Direction& outDir) const;
    
public:
    KeyBindingConfig();
    
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
    
    CommandType getCommandForKey(char key) const;
    Direction getDirectionForKey(char key) const;
    char getKeyForCommand(CommandType type) const;
    
    bool hasBinding(char key) const;
    bool isDirectionKey(char key) const;
    
    std::vector<std::pair<char, std::string>> getAllBindings() const;
    
    static KeyBindingConfig& getInstance();
};

#endif
