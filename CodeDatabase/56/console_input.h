#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H

#include "Command.h"
#include <optional>
#include <string>
#include <utility>

class ConsoleInput {
private:
    std::optional<std::pair<int, int>> directionToCoordinates(const std::string& direction);
    
public:
    ConsoleInput() = default;
    Command getCommand();
    

};

#endif