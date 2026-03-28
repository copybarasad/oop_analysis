#ifndef CONSOLEREADERH
#define CONSOLEREADERH
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../abstractNinterfaces/decision.h"
struct command {
    std::string name;
    std::vector<std::string> args;
    bool valid;

    command(std::string name, std::vector<std::string> args, bool valid) 
        : name(std::move(name)), args(std::move(args)), valid(valid) {
    }
};

class consoleReader : public decision{
public:
    command parseCommand(const std::string&);
};
#endif