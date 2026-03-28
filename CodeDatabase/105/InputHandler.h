#pragma once

#include "Command.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

class InputHandler {
public:

    struct ConfigLoadResult {
        bool success;
        std::unordered_map<char, CommandType> bindings;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
        std::vector<CommandType> missingCommands;
    };
    
    struct ConfigStatus {
        bool isValid;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
        std::vector<std::pair<std::string, std::string>> missingCommands;
    };
    
    struct RebindResult {
        bool success;
        std::string message;
        char oldKey;
        char newKey;
        std::string commandName;
    };

private:
    std::unordered_map<char, CommandType> keyBindings;
    std::string configFilePath;

    void clearInputBuffer();
    std::string getLineInternal();
    ConfigLoadResult loadConfigFromFile();
    void saveBindingsToFile(const std::unordered_map<char, CommandType>& bindings);
    char findKeyForCommand(CommandType command) const;
    std::string getCommandDescription(CommandType cmd) const;
    bool initializeKeyBindings();
    
public:
    InputHandler(const std::string& configFile = "controls.cfg");
    
    Command getGameCommand();
    Command getMenuCommand();
    Command getCoordinatesCommand();
    Command getSizeCommand();
    Command getSpellCommand(int maxSpells);
    bool getConfirmation();
    

    ConfigStatus getConfigStatus();
    std::vector<std::pair<std::string, std::vector<std::string>>> getCurrentBindings() const;
    std::string getControlsHelp() const;
    RebindResult rebindKey(CommandType command, char newKey);
    bool resetToDefaults();
    std::vector<std::pair<CommandType, std::string>> getCommandsForRebinding() const;
    char getKeyForCommand(CommandType command) const;
};