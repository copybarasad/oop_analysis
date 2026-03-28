#include "InputController/Handlers/console/ConsoleHandler.hpp"

Command ConsoleInput::getCommand() {
    std::string s;
    std::cin >> s;
    if (s == "g") return Command::New;
    if (s == "w") return Command::Up;
    if (s == "s") return Command::Down;
    if (s == "a") return Command::Left;
    if (s == "d") return Command::Right;
    return Command::None;
}

bool ConsoleInput::isAlive() {
    return true;
}
