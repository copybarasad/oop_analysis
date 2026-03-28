#include "ConsoleInput.h"
#include <iostream>

ConsoleInput::ConsoleInput() {
    bindings_.LoadFromFile("controls.cfg");
}

Command ConsoleInput::GetCommand() {
    char c;
    std::cin >> c;
    return bindings_.GetCommand(c);
}
