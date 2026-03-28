#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "command.h"
#include <iostream>
#include <string>
#include <map>

CommandType stringToCommand(const std::string& cmd);

class KeyBindings {
    std::map<char, CommandType> bindings;
public:
    void setDefault();
    bool load(const std::string& file);
    CommandType get(char key) const;
};

class ConsoleInput : public ICommandInput { 
    KeyBindings bindings;
public:
    ConsoleInput();
    Command getCommand() override;
};

#endif