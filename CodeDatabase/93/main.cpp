#include "game_view.h"
#include "console_renderer.h"
#include "console_input_parser.h"
#include "control_loader.h"
#include "save_manager.h"
#include "upgrade_system.h"
#include "game_level_manager.h"
#include <iostream>
#include <memory>
#include <exception>

std::unique_ptr<GameController> createNewGame() {
    int width = 15, height = 15, enemyCount = 3;

    std::cout << "Enter field width (10-25, default 15): ";
    std::cin >> width;
    if (width < 10 || width > 25) width = 15;

    std::cout << "Enter field height (10-25, default 15): ";
    std::cin >> height;
    if (height < 10 || height > 25) height = 15;

    std::cout << "Enter enemy count (3-5, default 3): ";
    std::cin >> enemyCount;
    if (enemyCount < 3 || enemyCount > 5) enemyCount = 3;

    std::cin.ignore(1000, '\n');
    return std::make_unique<GameController>(width, height, enemyCount);
}

int main() {
    try {
        std::cout << "Game created by Ivan Zaitsev\n\n";

        if (!SaveManager::saveExists("controls.cfg")) {
            ControlLoader::saveDefaultToFile("controls.cfg");
        }

        std::unique_ptr<GameController> game;

        std::cout << "Load saved game? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if ((choice == 'y' || choice == 'Y')) {
            std::cout << "Filename (default: save.txt): ";
            std::string filename;
            std::getline(std::cin, filename);
            if (filename.empty()) filename = "save.txt";

            if (SaveManager::saveExists(filename)) {
                try {
                    game = SaveManager::loadGame(filename);
                    std::cout << "Game loaded from " << filename << "!\n";
                } catch (const std::exception& e) {
                    std::cerr << "Failed to load: " << e.what() << "\n";
                }
            }
        }

        if (!game) {
            std::cout << "Starting new game...\n";
            game = createNewGame();
        }

        while (game && game->isGameRunning()) {
            GameView<ConsoleRenderer, ConsoleInputParser> view(*game);
            view.run();

            if (game->isGameWon()) {
                if (game->getCurrentLevel() >= 3) {
                    std::cout << "\nALL LEVELS COMPLETED!\nFinal Score: " 
                              << game->getPlayer().getScore() << "\n";
                    break;
                }

                UpgradeSystem::showUpgradeMenu(const_cast<Player&>(game->getPlayer()));
                GameLevelManager::advanceToNextLevel(*game);
                std::cout << "\nAdvancing to Level " << game->getCurrentLevel() << "...\n";
            } else {
                break;
            }
        }

        std::cout << "\nPress Enter to exit...";
        std::cin.ignore();
        std::cin.get();

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        std::cin.get();
        return 1;
    }

    return 0;
}