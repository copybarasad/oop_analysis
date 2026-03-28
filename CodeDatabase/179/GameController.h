#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "ICommandReader.h"  // Теперь включаем интерфейс
#include "Game.h"
#include "Logger.h"
#include <memory>

template<typename CommandReaderType>
class GameController {
private:
    std::unique_ptr<Game> game;
    std::unique_ptr<CommandReaderType> commandReader;
    
public:
    // Конструктор принимает любой тип, наследующий от ICommandReader
    GameController(std::unique_ptr<CommandReaderType> reader)
        : commandReader(std::move(reader)) {
        game = std::make_unique<Game>();
        LOG_INFO("GameController<" + std::string(typeid(CommandReaderType).name()) + "> initialized");
    }
    
    virtual ~GameController() {
        LOG_INFO("GameController destroyed");
    }
    
    void start() {
        LOG_INFO("Starting game");
        game->startNewGame();
        run();
    }
    
    void run() {
        LOG_INFO("Entering main game loop");
        
        while (game->isRunning()) {
            try {
                auto command = commandReader->readCommand();
                game->processCommand(command);
            } catch (const std::exception& e) {
                LOG_ERROR("Error processing command: " + std::string(e.what()));
            }
        }
        
        LOG_INFO("Game loop ended");
    }
    
    void setGame(std::unique_ptr<Game> newGame) {
        game = std::move(newGame);
        LOG_DEBUG("Game instance replaced");
    }
    
    Game* getGame() const {
        return game.get();
    }
    
    CommandReaderType* getCommandReader() const {
        return commandReader.get();
    }
};

#endif
