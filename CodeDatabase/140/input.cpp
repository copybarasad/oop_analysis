#include "input.h"
#include <iostream>
#include <limits>
#include <sstream>

void InputHandler::handleInput() {
    cout << "\nEnter command: ";
    string line;
    if (!std::getline(cin, line)) {
        running = false;
        return;
    }

    std::istringstream iss(line);
    string cmd;
    iss >> cmd;

    if (cmd == "q") {
        cout << "Exiting game.\n";
        running = false;
    } else if (cmd == "h") {
        command.display_help();
    } else if (cmd == "w" || cmd == "a" || cmd == "s" || cmd == "d") {
        command.move_command(cmd);
    } else if (cmd == "m") {
        command.mode_switch();
    } else if (cmd == "f") {
        int x, y;
        if (iss >> x >> y) {
            command.get_field().player_attack(x, y);
            cout << "Attacked position (" << x << ", " << y << ").\n";
            command.get_field().move_enemies();
            command.get_field().create_buildings();
            command.get_field().finish_turn();
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid coordinates!\n";
        }
    } else if (cmd == "u") {
        size_t idx;
        int x, y;
        if (iss >> idx >> x >> y) {
            command.get_field().player_use_spell(idx, x, y);
            cout << "Spell used!\n";
            command.get_field().move_enemies();
            command.get_field().create_buildings();
            command.get_field().finish_turn();
        } else {
            cout << "Usage: u <spell_index> <x> <y>\n";
        }
    } else if (cmd == "save") {
        command.get_game().save_game();
    } else if (cmd == "load") {
        command.get_game().load_game();
    } else {
        cout << "Unknown command. Type 'h' for help.\n";
    }
}