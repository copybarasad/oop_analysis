#include "command_reader.h"
#include "command_factory.h"
#include <iostream>
#include <limits>

std::unique_ptr<Command> CommandReader::GetPlayerCommand() {
    char input;
    
    std::cout << "Введите '?' в любой момент, чтобы посмотреть управление\n\n";
    std::cout << "\nКоманда: ";
    
    if (!(std::cin >> input)) {
        ClearInputBuffer();
        std::cout << "Неверный ввод!\n";
        return nullptr;
    }
    
    ClearInputBuffer();
    return CommandFactory::CreateCommandFromInput(input);
}

void CommandReader::ClearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
