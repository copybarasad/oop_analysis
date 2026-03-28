#include <iostream>
#include <string>
#include <memory>
#include "game_controller.h"
#include "game_save_manager.h"
#include "game_command_manager.h"
#include "game_visualization_manager.h"
#include "command_processor.h"
#include "game_renderer.h"
#include "command_reader.h"
#include "game_exceptions.h"
#include "command_interface.h" 
#include "command.h"
#include <limits>

// Прототипы функций
std::unique_ptr<GameController> createNewGame();
std::unique_ptr<GameController> loadGameFromFile(const std::string& filename);
void saveGameToFile(GameController* game);
void showGameOverMenu(GameController& game, GameVisualizationManager<GameRenderer>& visualizer);
void runGameLoop(GameController& game, 
                 CommandProcessor& processor,
                 GameCommandManager<CommandReader>& commandManager,
                 GameVisualizationManager<GameRenderer>& visualizer);

//Создает новую игру
std::unique_ptr<GameController> createNewGame() {
    auto game = std::make_unique<GameController>(15, 15, true);
    return game;
}

//Загружает игру из файла
std::unique_ptr<GameController> loadGameFromFile(const std::string& filename) {
    GameSaveManager saveManager;
    auto game = std::make_unique<GameController>(15, 15, false);
    int loadedLevel = 1;
    
    if (saveManager.loadGame(filename, 
        loadedLevel,
        game->getPlayer(),        
        game->getField(),         
        game->getEnemyManager(),  
        game->getBuildingManager(),
        game->getCombatManager(),
        game->getTowers())) {
        
        game->setCurrentLevel(loadedLevel);
        return game;
    }
    
    throw GameLoadException("Failed to load game data from: " + filename);
}

//Сохраняет игру в файл
void saveGameToFile(GameController* game) {
    if (!game) return;
    
    std::string filename;
    std::cout << "Enter save file name (without extension): ";
    std::cin >> filename;
    filename += ".sav";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    GameSaveManager saveManager;
    saveManager.saveGame(filename,
                        game->getCurrentLevel(),
                        game->getPlayer(),
                        game->getField(),
                        game->getEnemyManager(),
                        game->getBuildingManager(),
                        game->getCombatManager(),
                        game->getTowers());
    
    std::cout << "Game saved successfully as " << filename << std::endl;
}

//Показывает меню завершения игры
void showGameOverMenu(GameController& game, GameVisualizationManager<GameRenderer>& visualizer) {
    visualizer.onGameOver(game);
    
    char choice;
    std::cout << "Select option: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case '1':
            // Новая игра будет запущена в главном цикле
            break;
        case '2':
            // Возврат в главное меню
            break;
        case '3':
            exit(0);
        default:
            std::cout << "Returning to main menu..." << std::endl;
    }
}

//Основной игровой цикл
void runGameLoop(GameController& game, 
                 CommandProcessor& processor,
                 GameCommandManager<CommandReader>& commandManager,
                 GameVisualizationManager<GameRenderer>& visualizer) {
    
    bool gameRunning = true;
    int currentLevel = game.getCurrentLevel();
    
    // Отрисовываем старт уровня
    LevelConfig config{};
    config.level = currentLevel;
    config.fieldWidth = game.getField().getWidth();
    config.fieldHeight = game.getField().getHeight();
    visualizer.onLevelStart(currentLevel, config);
    
    while (gameRunning && game.isGameRunning()) {
        try {
            // Отрисовываем текущее состояние игры
            visualizer.onGameStateChanged(game);
            
            // Получаем команду
            std::cout << "Enter command: ";
            auto cmd = commandManager.getCommandInput().readInGameCommand(game);
            
            if (!cmd) {
                visualizer.showError("Invalid command!");
                continue;
            }
            
            // Выполняем команду
            bool continueGame = processor.processCommand(std::move(cmd));
            
            if (!continueGame) {
                gameRunning = false;
                break;
            }
            
            // Проверяем завершение уровня
            if (game.isLevelComplete()) {
                currentLevel++;
                visualizer.onLevelComplete(currentLevel - 1, game.getPlayer().getScore());
                visualizer.showMessage("Starting level " + std::to_string(currentLevel) + "...");
                game.startNextLevel();
                
                // Отрисовываем новый уровень
                if (game.isGameRunning()) {
                    LevelConfig newConfig{};
                    newConfig.level = currentLevel;
                    newConfig.fieldWidth = game.getField().getWidth();
                    newConfig.fieldHeight = game.getField().getHeight();
                    visualizer.onLevelStart(currentLevel, newConfig);
                }
            }
            
            // Отрисовываем обновленное состояние после хода
            visualizer.onTurnCompleted(game);
            
        } catch (const GameFileException& e) {
            visualizer.showError("File operation error: " + std::string(e.what()));
        } catch (const std::exception& e) {
            visualizer.showError("Error in game loop: " + std::string(e.what()));
            return;
        }
    }
    
    // Показываем меню завершения игры
    showGameOverMenu(game, visualizer);
}

//Главный цикл программы
void runMainLoop() {
    std::unique_ptr<GameController> currentGame;
    
    // Создаем менеджеры
    auto cmdManager = std::make_unique<GameCommandManager<CommandReader>>(std::cin, std::cout);
    auto vizManager = std::make_unique<GameVisualizationManager<GameRenderer>>(std::cout);
    CommandProcessor processor(nullptr);
    
    while (true) {
        try {
            // Отрисовываем главное меню
            vizManager->onMainMenuShown();
            
            // Получаем команду
            std::cout << "Select option: ";
            auto cmd = cmdManager->getCommandInput().readMainMenuCommand();
            
            if (!cmd) {
                vizManager->showError("Invalid option!");
                continue;
            }
            
            // Проверяем тип команды с помощью нового метода getCommandType()
            CommandType cmdType = cmd->getCommandType();
            
            switch (cmdType) {
                case CommandType::StartNewGame: {
                    // Создаем новую игру
                    currentGame = createNewGame();
                    processor.setGame(currentGame.get());
                    
                    // Запускаем игровой цикл
                    runGameLoop(*currentGame, processor, *cmdManager, *vizManager);
                    break;
                }
                    
                case CommandType::OpenLoadMenu: {
                    // Показываем меню загрузки
                    GameSaveManager saveManager;
                    auto saveFiles = saveManager.getSaveFiles();
                    
                    if (saveFiles.empty()) {
                        vizManager->showMessage("No save files available.");
                        break;
                    }
                    
                    vizManager->onLoadMenuShown(saveFiles);
                    
                    // Получаем команду загрузки
                    std::cout << "Select save file (0 to go back): ";
                    auto loadCmd = cmdManager->getCommandInput().readLoadMenuCommand(static_cast<int>(saveFiles.size()));
                    
                    if (!loadCmd || !loadCmd->isValid()) {
                        std::string error = loadCmd ? loadCmd->getErrorMessage() : "Invalid selection.";
                        vizManager->showError(error);
                        break;
                    }
                    
                    // Проверяем тип команды загрузки
                    CommandType loadCmdType = loadCmd->getCommandType();
                    
                    if (loadCmdType == CommandType::BackToMainMenu) {
                        // Возвращаемся в главное меню
                        break;
                    } else if (loadCmdType == CommandType::LoadSelectedSave) {
                        // Получаем индекс сохранения
                        int saveIndex = loadCmd->getCommandData();
                        if (saveIndex >= 0 && saveIndex < static_cast<int>(saveFiles.size())) {
                            try {
                                currentGame = loadGameFromFile(saveFiles[saveIndex]);
                                processor.setGame(currentGame.get());
                                vizManager->showMessage("Game loaded successfully!");
                                runGameLoop(*currentGame, processor, *cmdManager, *vizManager);
                            } catch (const std::exception& e) {
                                vizManager->showError(std::string("Load failed: ") + e.what());
                            }
                        }
                    }
                    break;
                }
                    
                case CommandType::QuitGame:
                    std::cout << "Goodbye!" << std::endl;
                    return;
                    
                default:
                    vizManager->showError("Unknown command type!");
                    break;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Error in main loop: " << e.what() << std::endl;
            vizManager->showError("Error: " + std::string(e.what()));
            currentGame.reset();
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        std::cout << "==========================================" << std::endl;
        std::cout << "          FANTASY BATTLE GAME" << std::endl;
        std::cout << "==========================================" << std::endl;
        
        // Запускаем главный цикл
        runMainLoop();
        
    } catch (const std::exception& e) {
        std::cerr << "\n*** FATAL ERROR: " << e.what() << " ***" << std::endl;
        std::cerr << "The game will now close." << std::endl;
        return 1;
    }
    
    std::cout << "Thank you for playing!" << std::endl;
    return 0;
}