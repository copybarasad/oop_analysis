#include "ConsoleInputHandler.h"
#include <iostream>
#include <string>

std::unique_ptr<Command> ConsoleInputHandler::getCommand() {
    std::cout << "Действие (w,a,s,d - перемещение, f - смена оружия, m - магия, p - магазин, k - сохранить, q - выйти): ";
    
    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) return std::make_unique<NoOpCommand>();

    char action = input[0];

    switch (action) {
        case 'w': return std::make_unique<MoveCommand>(0, -1);
        case 's': return std::make_unique<MoveCommand>(0, 1);
        case 'a': return std::make_unique<MoveCommand>(-1, 0);
        case 'd': return std::make_unique<MoveCommand>(1, 0);
        case 'f': return std::make_unique<ToggleWeaponCommand>();
        case 'p': return std::make_unique<ShopCommand>();
        case 'm': return std::make_unique<CastSpellCommand>();
        case 'k': return std::make_unique<SaveCommand>();
        case 'q': return std::make_unique<QuitCommand>();
        default:  return std::make_unique<NoOpCommand>();
    }
}