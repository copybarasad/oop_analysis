#include "Game.h"
#include "GameController.h"
#include "GameVisualizer.h"
#include "Renderer.h"
#include "InputReader.h"
#include "GameSaveManager.h"
#include "GameExceptions.h"
#include "Logger.h"
#include "EventBus.h"
#include <iostream>
#include <string>

void showMainMenu(bool hasSave) {
    std::cout << "\n=== OOP GAME ===\n";
    std::cout << "1. New Game\n";
    if (hasSave) {
        std::cout << "2. Load Game\n";
    } else {
        std::cout << "2. Load Game (No saved games)\n";
    }
    std::cout << "3. Exit\n";
    std::cout << "Choose (1-3): ";
}

template <typename InputReaderT, typename RendererT>
void runGameWithTemplates(Game& game, InputReaderT& reader, RendererT& renderer) {
    GameController<InputReaderT> controller(reader);
    GameVisualizer<Game, RendererT> visualizer(game, renderer);
    
    controller.runLoop(
        [&game](const Command& cmd) -> bool {
            return game.handleCommand(cmd);
        },
        [&game]() -> bool {
            return game.isGameRunning();
        },
        [&game]() {
            game.processEnemyTurns();
            game.processTurnEnd();
        },
        [&visualizer, &game]() {
            if (!game.canProcessInput()) {
                if (game.isGameRunning()) {
                    game.skipSlowedTurn();
                    game.processEnemyTurns();
                    game.processTurnEnd();
                }
            }
            visualizer.draw();
        }
    );
}

int main(int argc, char** argv) {
    try {
        std::string logMode = "console";
        std::string logFile = "game.log";
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--log=file") logMode = "file";
            if (arg.rfind("--log-file=", 0) == 0) logFile = arg.substr(11);
            if (arg == "--log=console") logMode = "console";
        }

        auto bus = std::make_shared<EventBus>();
        std::shared_ptr<ILogger> logger = logMode == "file" 
            ? LoggerFactory::createFile(logFile) 
            : LoggerFactory::createConsole();
        bus->subscribe(logger);

        GameSaveManager saveManager;
        
        while (true) {
            bool hasSave = saveManager.hasSaveFile();
            showMainMenu(hasSave);
            
            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::string dummy;
                std::getline(std::cin, dummy);
                continue;
            }
            
            if (choice == 1) {
                saveManager.deleteSaveFile();
                saveManager.deleteAutoSave();
                std::cout << "\nStarting new game...\n";
                
                Game game;
                game.setEventBus(bus);
                game.initializeGame();
                
                ConsoleInputReader reader(game.getInputConfig());
                ConsoleRenderer renderer;
                runGameWithTemplates(game, reader, renderer);
                
            } else if (choice == 2 && hasSave) {
                try {
                    std::cout << "\nLoading game...\n";
                    Game game;
                    game.setEventBus(bus);
                    if (game.loadGame()) {
                        ConsoleInputReader reader(game.getInputConfig());
                        ConsoleRenderer renderer;
                        runGameWithTemplates(game, reader, renderer);
                    } else {
                        std::cout << "Failed to load game. Press Enter to continue...";
                        std::cin.ignore();
                        std::cin.get();
                    }
                } catch (const SaveLoadException& e) {
                    std::cout << "Failed to load game: " << e.what() << "\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                }
                
            } else if (choice == 3) {
                std::cout << "Thanks for playing!\n";
                saveManager.deleteAutoSave();
                break;
            }
        }
        
        saveManager.deleteAutoSave();
        
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
