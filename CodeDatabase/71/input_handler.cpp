#include "input_handler.h"
#include <iostream>
#include <limits>

Command ConsoleInputHandler::getCommand() {
    char input;
    std::cin >> input;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (input) {
    case 'w': return Command(CommandType::MOVE_UP);
    case 'a': return Command(CommandType::MOVE_LEFT);
    case 's': return Command(CommandType::MOVE_DOWN);
    case 'd': return Command(CommandType::MOVE_RIGHT);
    case 'c': return Command(CommandType::USE_SPELL);
    case 'b': return Command(CommandType::OPEN_SHOP);
    case 'p': return Command(CommandType::SAVE_GAME);
    case 'q': return Command(CommandType::QUIT);
    default:  return Command(CommandType::UNKNOWN);
    }
}