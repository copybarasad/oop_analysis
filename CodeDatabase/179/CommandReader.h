#ifndef COMMANDREADER_H
#define COMMANDREADER_H

#include "GameCommand.h"
#include <map>
#include <string>
#include <functional>
#include <memory>
#include <iostream>

class ICommandReader {
public:
    virtual ~ICommandReader() = default;
    virtual GameCommand readCommand() = 0;
    virtual void displayHelp() = 0;
};

class ConsoleCommandReader : public ICommandReader {
private:
    std::map<std::string, CommandType> keyBindings;
    std::map<CommandType, std::string> commandDescriptions;
    
public:
    ConsoleCommandReader();
    ConsoleCommandReader(const std::string& configFile);
    
    GameCommand readCommand() override;
    void displayHelp() override;
    
    void loadKeyBindings(const std::string& configFile);
    void setDefaultBindings();
    
private:
    void initializeDescriptions();
};

#endif
