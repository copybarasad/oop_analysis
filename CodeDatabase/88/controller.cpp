#include "controller.hpp"


template<>
GameController<ConsoleInputHandler>::GameController(Game* gameInstance, const std::string& configFile) 
    : game(gameInstance) {
    if (!configFile.empty()) {
        if (!inputHandler.loadFromFile(configFile)) {
            std::cout << "Using default commands.\n";
        }
    }
}


template<>
void GameController<ConsoleInputHandler>::setOnGameStateChanged(std::function<void()> callback) {
    onGameStateChanged = callback;
}

template<>
std::unique_ptr<Command> GameController<ConsoleInputHandler>::getPlayerCommand() {
    std::cout << "Enter command: ";
    std::string input;
    std::getline(std::cin, input);
    
    return inputHandler.parseCommand(input);
}

template<>
CommandResult GameController<ConsoleInputHandler>::processCommand(Command* command) {
    if (!command) {
        return CommandResult::UnknownCommand;
    }
    
    if (command->getName() == "help") {
        inputHandler.printAvailableCommands();
        return CommandResult::Success;
    }
    
    command->execute(*game);
    
    if (command->getName() == "end_game") {
        return CommandResult::GameEnd;
    } else if (command->getName() == "save_game") {
        return CommandResult::SaveGame;
    }
    
    return CommandResult::Success;
}


template<>
void GameController<ConsoleInputHandler>::runGameLoop() {
    bool running = true;
    
    while (running && game && !game->isGameOver()) {
        
        if (game->playerWon()) {
            if (!game->advanceToNextLevel()) {
                break;
            }
            
            if (onGameStateChanged) {
                onGameStateChanged();
            }
            continue; 
        }
        
        auto command = getPlayerCommand();
        CommandResult result = processCommand(command.get());
        
        switch (result) {
            case CommandResult::GameEnd:
                running = false;
                break;
            case CommandResult::SaveGame:
                std::cout << "Game saved successfully!\n";
                break;
            case CommandResult::UnknownCommand:
                std::cout << "Unknown command. Type 'help' for available commands.\n";
                break;
            case CommandResult::Success:
                break;
        }
        
        if (running && result != CommandResult::UnknownCommand) {
            game->processEnemyTurns();
            game->updateGameState();
            
            if (onGameStateChanged) {
                onGameStateChanged();
            }
        }
    }
}