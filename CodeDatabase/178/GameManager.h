#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "IInputStrategy.h"
#include "GameController.h"
#include <memory>
#include <iostream>

template<typename InputStrategy>
class GameManager {
private:
    GameController& gameController;
    InputStrategy inputStrategy;
    bool isRunning;
    
public:
    GameManager(GameController& controller, const std::string& configFile = "")
        : gameController(controller), isRunning(true) {
        
        if (!configFile.empty()) {
            try {
                loadConfigIfAvailable(configFile);
            } catch (const std::exception& e) {
                std::cerr << "Warning: Could not load config from '" << configFile 
                          << "': " << e.what() << std::endl;
                std::cout << "Using default configuration." << std::endl;
            }
        }
    }
    
    std::unique_ptr<ICommand> processInput(char input) {
        if (!inputStrategy.isValidInput(input)) {
            std::cout << "Invalid input! Press 'H' for help." << std::endl;
            return nullptr;
        }
        
        auto command = inputStrategy.createCommand(input, gameController);
        return command;
    }
    
    void executeCommand(ICommand& command) {
        try {
            command.execute(gameController);
            
            if (!gameController.isGameRunning()) {
                isRunning = false;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error executing command: " << e.what() << std::endl;
            throw;
        }
    }
    
    void showHelp() {
        inputStrategy.showHelp();
    }
    
    bool isGameRunning() const {
        return isRunning && gameController.isGameRunning();
    }
    
    void stop() {
        isRunning = false;
    }
    
    InputStrategy& getInputStrategy() {
        return inputStrategy;
    }
    
    void loadConfig(const std::string& filename) {
        std::cout << "GameManager: Attempting to load config from " << filename << std::endl;
        loadConfigIfAvailable(filename);
    }
    
private:
    void loadConfigIfAvailable(const std::string& filename) {
        try {
            inputStrategy.loadConfigFromFile(filename);
            std::cout << "Configuration loaded successfully from '" << filename << "'" << std::endl;
        } catch (...) {
            std::cout << "Note: Input strategy doesn't support config file loading." << std::endl;
        }
    }
};

#endif