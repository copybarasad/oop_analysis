#pragma once
#include <map>
#include <string>
#include "Command.h"

class ConsoleInputHandler {
private:
    std::map<char, CommandType> key_map;
    std::string input_prompt;

public:
    ConsoleInputHandler();
    void load_config();
    Command get_command();
    int get_int(const std::string& prompt);
    std::string get_string(const std::string& prompt);
};
