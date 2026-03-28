#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <memory>
#include <string>
#include <map>
#include "command.hpp"

class InputHandler {
public:
    virtual ~InputHandler() = default;
    virtual std::unique_ptr<Command> parseCommand(const std::string& input) = 0;
    virtual void printAvailableCommands() const = 0;
    virtual bool loadFromFile(const std::string& filename) = 0;
};

#endif