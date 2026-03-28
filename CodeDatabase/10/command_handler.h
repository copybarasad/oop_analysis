#pragma once
#include <memory>
#include <string>
#include <map>
#include "command.h"
#include <vector>

class CommandHandler {
private:
    static std::map<std::string, std::string> keyToCommand;
    
    static void loadDefaultConfig();
    static bool validateConfig();
    
public:
    static void loadConfig(const std::string& configFile = "game_config.txt");
    static std::unique_ptr<Command> parseCommand(const std::string& input);
};
