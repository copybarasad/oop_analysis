#include "ConsoleInputAdapter.h"
#include <iostream>
#include <cctype>

bool ConsoleInputAdapter::handleInput(char input, Player& player, GameField& field, 
                                     Enemy& enemy, EnemyManager& enemyManager, LevelManager& levelManager) {
    char lowerInput = std::tolower(input);
     
    auto command = commandProcessor.createCommand(lowerInput);
    if (command) {
        std::cout << "Выполняется команда: " << command->getDescription() << std::endl;
        bool result = command->execute(player, field, enemy, enemyManager, levelManager);
        return result && command->consumesTurn();
    }
     
    std::cout << "Неизвестная команда: '" << input << "'\n";
    std::cout << "Введите 'h' для справки по командам\n";
    return false;
}

void ConsoleInputAdapter::printHelp() {
    commandProcessor.printAvailableCommands();
    
    std::cout << "\n=== СПЕЦИАЛЬНЫЕ КОМАНДЫ ===\n";
    std::cout << "h - показать эту справку\n";
    std::cout << "r - перезагрузить конфигурацию управления\n";
    std::cout << "============================\n";
}

bool ConsoleInputAdapter::reloadConfig() {
    return commandProcessor.reloadKeybindings();
} 