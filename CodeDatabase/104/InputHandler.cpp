#include "InputHandler.h"
#include "Player.h"   
#include "GameField.h"
#include "MeleeSpell.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Парсинг ввода пользователя
InputHandler::PlayerInput InputHandler::parseInput(const std::string& input) const {
    PlayerInput result;
    result.type = PlayerInput::Type::INVALID;
    result.move_dx = 0;
    result.move_dy = 0;

    std::stringstream ss(input);
    std::string command;
    ss >> command;

    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (command == "w" || command == "a" || command == "s" || command == "d") {
        result.type = PlayerInput::Type::MOVE;

        if (command == "w") {
            result.move_dx = 0;
            result.move_dy = -1;
        }
        else if (command == "s") {
            result.move_dx = 0;
            result.move_dy = 1;
        }
        else if (command == "a") {
            result.move_dx = -1;
            result.move_dy = 0;
        }
        else if (command == "d") {
            result.move_dx = 1;
            result.move_dy = 0;
        }
    }
    else if (command == "cast" || command == "c") {
        result.type = PlayerInput::Type::CAST_SPELL;
        if (!(ss >> result.spell_index)) {
            result.type = PlayerInput::Type::INVALID;
        }
        else {
            result.spell_index--;
        }
    }
    else if (command == "quit" || command == "q") {
        result.type = PlayerInput::Type::QUIT;
    }

    return result;
}

SpellCastInfo InputHandler::getSpellTarget(const Player& player, const GameField& field) const {
    int target_x, target_y;

    std::cout << "Enter target coordinates for spell (x y): ";
    std::cin >> target_x >> target_y;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return SpellCastInfo(Position(target_x, target_y));
}
