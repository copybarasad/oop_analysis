#include "consoleinput.hpp"
#include <iostream>

Command ConsoleInput::read_command() {
    std::cout << "Action: ";
    std::string input;
    std::cin >> input;
    if (input.empty()) return Command(GameAction::INVALID);

    char key = input[0];
    if (keymap.count(key) == 0) return Command(GameAction::INVALID);

    GameAction act = keymap.at(key);
    switch (act) {
        case GameAction::CAST_SPELL: {
            int idx, tx, ty;
            std::cout << "Spell index: "; std::cin >> idx;
            std::cout << "Target x y: "; std::cin >> tx >> ty;
            return Command(act, idx, tx, ty);
        }
        case GameAction::SAVE: {
            std::string name;
            std::cout << "Save filename: "; std::cin >> name;
            return Command(act, name);
        }
        default:
            return Command(act);
    }
}