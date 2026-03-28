#ifndef TEMPLATE_MENU_H
#define TEMPLATE_MENU_H

#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <unistd.h>

#include "GameController.h"
#include "GameVisualizer.h"

class Game;
class Command;

#include "GameReader.h"
#include "GameSaver.h"
#include "SaveFileManager.h"


namespace fs = std::filesystem;

template<typename InputHandlerType,
         typename RendererType>
class TemplateMenu {
    std::string directory;
    std::unique_ptr<GameVisualizer<RendererType>> visualizer;
    SaveFileManager fileManager;

public:
    TemplateMenu(const std::string &directory = "saves")
        : directory(directory), fileManager(directory)
    {
        if (!fs::exists(directory)) {
            fs::create_directory(directory);
        }

        auto renderer = std::make_unique<RendererType>();
        visualizer = std::make_unique<GameVisualizer<RendererType>>(std::move(renderer));
    }

    void showMain() {
        while (true) {
            std::vector<std::string> menuItems = {
                "Новая игра",
                "Загрузить игру",
                "Выйти"
            };

            visualizer->renderMainMenu(menuItems);

            int choice;
            std::cin >> choice;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (choice) {
                case 1: startNewGame(); break;
                case 2: showLoadMenu(); break;
                case 3: return;
                default:
                    visualizer->renderMessage("Ошибка", "Неверный выбор!");
                    std::cin.get();
                    break;
            }
        }
    }

private:
    void startNewGame() {
        try {
            sleep(1);

            LevelManager levelManager(1);
            auto game = std::make_shared<Game>(nullptr, nullptr, levelManager);

            bool shouldSave = runGameWithTemplateComponents(game);

            if (shouldSave) {
                saveGame(game);
            }
        } catch (const std::exception& e) {
            visualizer->renderMessage("Ошибка", std::string("Ошибка при создании игры: ") + e.what());
            sleep(2);
        }
    }

    void showLoadMenu() {
        auto saves = fileManager.getSortedSaveFiles();
        if (saves.empty()) {
            visualizer->renderMessage("Ошибка", "Нет сохраненных игр!");
            sleep(2);
            return;
        }

        std::vector<std::string> saveNames;
        for (const auto& save : saves) {
            saveNames.push_back(fileManager.formatSaveFileDisplay(save));
        }

        visualizer->renderLoadMenu(saveNames);

        int choice;
        std::cin >> choice;

        if (choice == 0) {
            return;
        }

        if (choice < 1 || choice > static_cast<int>(saves.size())) {
            visualizer->renderMessage("Ошибка", "Неверный выбор!");
            sleep(2);
            return;
        }

        std::string filename = saves[choice - 1].string();

        try {
            GameReader reader(filename);
            auto game = reader.readGame();

            sleep(1);

            bool shouldSave = runGameWithTemplateComponents(game);

            if (shouldSave) {
                saveGame(game);
            }
        } catch (const std::exception& e) {
            visualizer->renderMessage("Ошибка", std::string("Ошибка загрузки: ") + e.what());
            sleep(2);
        }
    }

    bool runGameWithTemplateComponents(std::shared_ptr<Game> game) {
        auto inputHandler = std::make_unique<InputHandlerType>();
        auto renderer = std::make_unique<RendererType>();

        auto gameController = std::make_unique<GameController<InputHandlerType>>(
            game.get(),
            std::move(inputHandler)
        );

        auto gameVisualizer = std::make_unique<GameVisualizer<RendererType>>(
            std::move(renderer)
        );

        if (!gameController || !gameVisualizer) {
            visualizer->renderMessage("Ошибка", "Ошибка создания контроллеров");
            return false;
        }

        while (game->isPlayerAlive() && gameController->isGameRunning()) {
            system("clear");

            if (game->isLevelComplete()) {
                game->advanceToNextLevel();
            }

            gameVisualizer->render(*game->getField());


            if (auto player = game->getPlayer(); player && player->isSkipStep()) {
                game->handleSkipStep();
                continue;
            }

            auto command = gameController->getNextCommand();
            if (!command) {
                continue;
            }

            CommandResult result = command->execute(*game);

            if (!result.message.empty()) {
                std::cout << "\n" << result.message << std::endl;
                sleep(1);
            }

            if (result.shouldExit) {
                if (result.shouldSave) {
                    visualizer->renderMessage("Выход", "Игра будет сохранена перед выходом...");
                    sleep(1);
                }
                return result.shouldSave;
            }

            if (result.moveEnemies) {
                game->updateGameState();
            }

            sleep(1);
        }

        gameVisualizer->renderGameOver(*game->getField());
        std::cin.get();

        return false;
    }

    void saveGame(std::shared_ptr<Game> game) {
        if (!game) {
            visualizer->renderMessage("Ошибка", "Нет игры для сохранения!");
            return;
        }

        std::string filename = fileManager.generateSaveFileName();

        try {
            GameSaver saver(filename);
            saver.saveGame(game);

            visualizer->renderSaveMessage(filename);
            std::cin.get();
        } catch (const std::exception& e) {
            visualizer->renderMessage("Ошибка", std::string("Ошибка сохранения: ") + e.what());
            sleep(2);
        }
    }
};

#endif // TEMPLATE_MENU_H