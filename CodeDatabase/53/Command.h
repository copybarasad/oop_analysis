#ifndef COMMAND_H
#define COMMAND_H
#include <string>


class Command {

public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void parse(const std::string& input) = 0;
    virtual std::string getName() const = 0;
};

#endif