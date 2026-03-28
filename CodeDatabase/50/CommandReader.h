#pragma once
#include <map>
#include <string>
#include <iostream>
#include "Command.h"
#include <limits>

class CommandReader {
public:
    explicit CommandReader(const std::string& configFile = "controls.cfg");

    // Считывает клавишу и возвращает объект команды
    Command readCommand();

    // Печать текущего управления
    void printBindingsHelp() const;
    void printMenuBindings() const;
    void printGameBindings() const;


private:
    std::map<char, CommandType> keyToCommand;

    void setDefaultBindings();
    bool loadFromFile(const std::string& filename);

    static CommandType commandFromName(const std::string& name);
    static std::string commandToName(CommandType cmd);
    static std::string trim(const std::string& s);
};
