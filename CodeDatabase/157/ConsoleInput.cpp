#include "ConsoleInput.h"
#include <iostream>
#include <sstream>
#include <cctype>

Command ConsoleInput::nextCommand() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        Command c; c.action = Action::Quit; return c;
    }
    size_t p = 0;
    while (p < line.size() && std::isspace(static_cast<unsigned char>(line[p]))) ++p;
    if (p >= line.size()) return Command{};
    return parseLine(line.substr(p));
}

Command ConsoleInput::parseLine(const std::string &s) const {
    std::istringstream ss(s);
    std::string cmd;
    ss >> cmd;
    Command c;
    if (cmd == "move") {
        c.action = Action::Move; ss >> c.x >> c.y; if (ss.fail()) c.action = Action::None;
    } else if (cmd == "attack") {
        c.action = Action::Attack; ss >> c.x >> c.y; if (ss.fail()) c.action = Action::None;
    } else if (cmd == "use") {
        c.action = Action::Use; ss >> c.idx >> c.x >> c.y; if (ss.fail()) c.action = Action::None;
    } else if (cmd == "save") {
        c.action = Action::Save; ss >> c.arg; if (c.arg.empty()) c.action = Action::None;
    } else if (cmd == "load") {
        c.action = Action::Load; ss >> c.arg; if (c.arg.empty()) c.action = Action::None;
    } else if (cmd == "next") {
        c.action = Action::Next;
    } else if (cmd == "quit" || cmd == "exit") {
        c.action = Action::Quit;
    } else {
        c.action = Action::None;
    }
    return c;
}
