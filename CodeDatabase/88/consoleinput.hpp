#ifndef CONSOLEINPUTHANDLER_HPP
#define CONSOLEINPUTHANDLER_HPP

#include "inputhandler.hpp"
#include "command.hpp"
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include "../include/json.hpp"

using json = nlohmann::json;

class ConsoleInputHandler : public InputHandler {
private:
    std::map<std::string, std::string> commandMap; 
    
public:
    ConsoleInputHandler() {loadDefaultCommands();}
    std::unique_ptr<Command> parseCommand(const std::string& input) override;
    void printAvailableCommands() const override;
    bool loadFromFile(const std::string& filename) override;
    const std::map<std::string, std::string>& getCommandMap() const;
    
private:
    void loadDefaultCommands();
};

#endif