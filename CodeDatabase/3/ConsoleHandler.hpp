#include "InputController/commands.hpp"
#include <iostream>
#include "GameClass/GameClass.hpp"

class ConsoleInput {
public:
    ConsoleInput() = default;
    Command getCommand();
    bool isAlive();
};
