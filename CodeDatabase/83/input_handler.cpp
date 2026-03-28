#include "input_handler.h"
#include <conio.h>
#include <fstream>
#include <sstream>
#include <iostream>

InputHandler::InputHandler(const std::string& configFile) {
    loadConfig(configFile);
}

const std::vector<Command> InputHandler::requiredCommands = {
    Command::MENU_NEW_GAME,
    Command::MENU_LOAD_GAME,
    Command::MENU_EXIT,
    Command::MOVE_UP,
    Command::MOVE_DOWN,
    Command::MOVE_LEFT,
    Command::MOVE_RIGHT,
    Command::ATTACK,
    Command::CHANGE_SPELL,
    Command::SAVE_GAME,
    Command::EXIT_TO_MENU
};


Command InputHandler::processInput() {
    char c = _getch();

    auto it = keymap.find(c);
    if (it != keymap.end()) {
        return it->second;
    }

    return Command::NONE;
}

void InputHandler::loadConfig(const std::string& filename) {
    std::ifstream file(filename);

    std::unordered_map<char, Command> tempKeymap;
    std::unordered_map<Command, char> reverseMap;

    if (!file.is_open()) {
        setDefaultControls();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        if (pos == 0 || pos + 1 >= line.size())
            continue;

        char key = line[0];
        Command cmd = commandFromString(line.substr(pos + 1));

        if (cmd == Command::NONE) {
            setDefaultControls();
            return;
        }

        if (tempKeymap.count(key)) {
            setDefaultControls();
            return;
        }

        if (reverseMap.count(cmd)) {
            setDefaultControls();
            return;
        }

        tempKeymap[key] = cmd;
        reverseMap[cmd] = key;
    }

    for (Command cmd : requiredCommands) {
        if (!reverseMap.count(cmd)) {
            setDefaultControls();
            return;
        }
    }

    keymap = std::move(tempKeymap);
}


Command InputHandler::commandFromString(const std::string& str) {
    if (str == "MENU_NEW_GAME") return Command::MENU_NEW_GAME;
    if (str == "MENU_LOAD_GAME") return Command::MENU_LOAD_GAME;
    if (str == "MENU_EXIT") return Command::MENU_EXIT;

    if (str == "MOVE_UP") return Command::MOVE_UP;
    if (str == "MOVE_DOWN") return Command::MOVE_DOWN;
    if (str == "MOVE_LEFT") return Command::MOVE_LEFT;
    if (str == "MOVE_RIGHT") return Command::MOVE_RIGHT;

    if (str == "ATTACK") return Command::ATTACK;
    if (str == "TARGET_MODE") return Command::TARGET_MODE;
    if (str == "CHANGE_SPELL") return Command::CHANGE_SPELL;
    if (str == "SAVE_GAME") return Command::SAVE_GAME;
    if (str == "EXIT_TO_MENU") return Command::EXIT_TO_MENU;

    return Command::NONE;
}


void InputHandler::setDefaultControls() {
    keymap.clear();

    keymap['1'] = Command::MENU_NEW_GAME;
    keymap['2'] = Command::MENU_LOAD_GAME;
    keymap['3'] = Command::MENU_EXIT;

    keymap['w'] = Command::MOVE_UP;
    keymap['s'] = Command::MOVE_DOWN;
    keymap['a'] = Command::MOVE_LEFT;
    keymap['d'] = Command::MOVE_RIGHT;

    keymap['f'] = Command::ATTACK;
    keymap['c'] = Command::CHANGE_SPELL;
    keymap['v'] = Command::SAVE_GAME;
    keymap['q'] = Command::EXIT_TO_MENU;
}

