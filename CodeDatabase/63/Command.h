#ifndef RPG_COMMAND_H
#define RPG_COMMAND_H

#include <utility>
#include <string>

class Command {
public:
    virtual ~Command() = default;

    virtual std::pair<bool, std::string> execute() = 0;
};

#endif