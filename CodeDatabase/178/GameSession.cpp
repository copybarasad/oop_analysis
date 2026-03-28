#include "GameSession.h"
#include "Game.h"
#include "Logger.h"
#include <iostream>
#include <limits>
#include <cctype>

GameSession::GameSession() : sessionActive(false) {}

void GameSession::run(Game& game) {
    if (!controller || !levelManager) {
        game.setState(GameState::MAIN_MENU);
        return;
    }
    initializeManagers();
    Logger::getInstance().logInfo("Game session started");
    controller->notifyGameStarted();
    sessionActive = true;
    
    while (sessionActive && controller && controller->isGameRunning()) {
        renderManager->clearScreen();
        renderManager->renderGame(*controller);
        
        if (SaveSystem::saveExists()) {
            std::cout << " [!] Save file exists - press 'L' to load" << std::endl;
        }
        
        std::cout << "\nEnter command (H for help, V to save, Q to quit): ";
        char input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        Logger::getInstance().logDebug(std::string("User input: '") + input + "'");
        
        input = std::tolower(input);
        
        switch (input) {
            case 'q':
                sessionActive = false;
                controller->notifyGameEnded();
                game.setState(GameState::MAIN_MENU);
                break;
                
            case 'v':
                if (saveGame()) {
                    std::cout << "\n=== GAME SAVED SUCCESSFULLY ===" << std::endl;
                    std::cout << "Game progress has been saved to file." << std::endl;
                } else {
                    std::cout << "\n=== SAVE FAILED ===" << std::endl;
                    std::cout << "Could not save the game. Check logs for details." << std::endl;
                }
                
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
                
            case 'l':
                if (SaveSystem::saveExists()) {
                    std::cout << "\nLoad saved game? (y/n): ";
                    char confirm;
                    std::cin >> confirm;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    if (std::tolower(confirm) == 'y') {
                        if (loadGame()) {
                            std::cout << "\n=== GAME LOADED ===" << std::endl;
                            std::cout << "Save game loaded successfully!" << std::endl;
                            std::cout << "\nPress Enter to continue...";
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            continue;
                        }
                    }
                }
                break;
                
            default:
                break;
        }
        
        if (!sessionActive) break;
        auto command = gameManager->processInput(input);
        
        if (command) {
            controller->notifyCommandExecuted(command->toString());
            gameManager->executeCommand(*command);
        } else if (input == 'h') {
            gameManager->showHelp();
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (!std::isspace(input) && input != 'v' && input != 'q' && input != 'l') {
            std::cout << "Unknown command! Press 'H' for help." << std::endl;
            Logger::getInstance().logWarning(std::string("Unknown command: '") + input + "'");
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        checkGameState(game);
    }
}

void GameSession::initializeManagers() {
    if (!controller) {
        Logger::getInstance().logError("Cannot initialize managers: controller is null");
        return;
    }
    
    try {
        std::string configFile = "controls.config";
        gameManager = std::make_unique<GameManager<ConsoleInputStrategy>>(*controller, configFile);
        auto renderStrategy = std::make_unique<ConsoleRenderStrategy>();
        renderManager = std::make_unique<RenderManager<ConsoleRenderStrategy>>(std::move(renderStrategy));
        
        auto loggerPtr = Logger::getInstancePtr();
        if (loggerPtr) {
            controller->addObserver(loggerPtr);
            Logger::getInstance().logInfo("Logger added as observer to GameController");
        }
        
        Logger::getInstance().logInfo("Game managers initialized successfully");
        
    } catch (const std::exception& e) {
        Logger::getInstance().logError(std::string("Failed to initialize managers: ") + e.what());
        throw;
    }
}

void GameSession::startNewGame() {
    try {
        LevelConfig config = GameConfig::getLevelConfig(1);
        controller = std::make_unique<GameController>(config.fieldWidth, config.fieldHeight);
        levelManager = std::make_unique<LevelManager>();
        levelManager->initializeLevel(*controller);
        sessionActive = true;
        
        Logger::getInstance().logInfo("New game started - Level 1");
        
    } catch (const std::exception& e) {
        Logger::getInstance().logError(std::string("Failed to start new game: ") + e.what());
        throw;
    }
}

void GameSession::cleanup() {
    if (controller) {
        auto loggerPtr = Logger::getInstancePtr();
        if (loggerPtr) {
            controller->removeObserver(loggerPtr);
        }
    }
    
    gameManager.reset();
    renderManager.reset();
    controller.reset();
    levelManager.reset();
    sessionActive = false;
    
    Logger::getInstance().logInfo("Game session cleaned up");
}

void GameSession::setController(std::unique_ptr<GameController> newController) {
    controller = std::move(newController);
    if (controller) {
        auto loggerPtr = Logger::getInstancePtr();
        if (loggerPtr) {
            controller->addObserver(loggerPtr);
        }
    }
}

void GameSession::setLevelManager(std::unique_ptr<LevelManager> newLevelManager) {
    levelManager = std::move(newLevelManager);
}

void GameSession::checkGameState(Game& game) {
    if (!controller || !levelManager) {
        game.setState(GameState::MAIN_MENU);
        return;
    }
    levelManager->checkLevelCompletion(*controller);
    
    if (levelManager->isLevelCompleted()) {
        Logger::getInstance().logInfo("Level completed, transitioning to next level");
        game.setState(GameState::LEVEL_TRANSITION);
        sessionActive = false;
    }
    if (levelManager->isGameCompleted()) {
        Logger::getInstance().logInfo("Game completed - VICTORY!");
        game.setState(GameState::VICTORY);
        sessionActive = false;
    }
    if (!controller->isGameRunning()) {
        Logger::getInstance().logInfo("Player died - GAME OVER");
        game.setState(GameState::GAME_OVER);
        sessionActive = false;
    }
}

bool GameSession::saveGame() {
    if (!canSave()) {
        Logger::getInstance().logError("Cannot save: game not properly initialized");
        return false;
    }
    try {
        SaveSystem::saveGame(*controller, *levelManager);
        controller->notifyGameSaved();
        Logger::getInstance().logInfo("Game saved successfully");
        return true;
    } catch (const SaveException& e) {
        Logger::getInstance().logError(std::string("Save failed: ") + e.what());
        return false;
    }
}

bool GameSession::loadGame() {
    try {
        auto [loadedController, loadedLevelManager] = SaveSystem::loadGame();
        cleanup();
        controller = std::move(loadedController);
        levelManager = std::move(loadedLevelManager);
        initializeManagers();
        Logger::getInstance().logInfo("Game loaded successfully");
        return true;
    } catch (const LoadException& e) {
        Logger::getInstance().logError(std::string("Load failed: ") + e.what());
        return false;
    }
}