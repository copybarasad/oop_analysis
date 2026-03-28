#include "game.h"
#include "game_controller.h"
#include "game_visualizer.h"
#include "console_input_reader.h"
#include "console_renderer.h"
#include "game_state_manager.h"
#include "game_end_handler.h"
#include "level_manager.h"
#include "upgrade_system.h"
#include "logging_config.h"
#include "event_bus.h"
#include "game_event.h"
#include "exceptions.h"
#include "action_commands.h"
#include "config_manager.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // Инициализация логирования
        LoggingConfig loggingConfig = LoggingManager::parseArgs(argc, argv);
        LoggingManager::initializeLogging(loggingConfig);
        
        // Опубликовать событие начала игры
        EventBus::getInstance().publish(std::make_shared<GameStartedEvent>(1));
        
        // Инициализация конфигурации управления
        ConfigManager::initializeConfig("controls.cfg");
        
        // Создать игру
        Game game;
        
        // Создать контроллер (шаблонный класс с ConsoleInputReader)
        // Контроллер преобразует ввод в команды
        GameController<ConsoleInputReader> controller(game, "controls.cfg");
        
        // Создать визуализатор (шаблонный класс с ConsoleRenderer)
        // Визуализатор отрисовывает игру на основе событий
        GameVisualizer<ConsoleRenderer> visualizer(game);
        
        // Главный игровой цикл
        while (true) {
            std::cout << "=== NEW GAME / LOAD OR START ===\n";

            // Спросить, загружать ли сохраненную игру
            bool loadChoice = controller.getInputReader().getYesNoChoice("Load saved game? (y/n): ");
            if (loadChoice) {
                std::string fname = controller.getInputReader().getFilename("Enter save filename: ");
                GameStateManager stateManager(game);
                if (!stateManager.loadGame(fname)) {
                    std::cout << "Failed to load: " << game.getLastError() << "\n";
                    std::cout << "Starting new game instead.\n";
                    game.initializeGame();
                } else {
                    std::cout << "Loaded " << fname << "\n";
                }
            } else {
                std::cout << "Starting new game...\n";
                game.initializeGame();
            }

            // Отрисовать управление и поле
            visualizer.renderControls();
            visualizer.renderField();

            // Цикл игровых ходов
            while (!game.isGameOver()) {
                // Обновить отображение статуса
                visualizer.update();

                // Проверить, жив ли игрок
                if (!game.player.isAlive()) {
                    game.gameOver = true;
                    break;
                }

                try {
                    // Получить следующую команду от контроллера
                    // Контроллер использует inputReader для получения GameCommand
                    // и преобразует её в Command объект
                    auto command = controller.getNextCommand();

                    // Проверить, это команда выхода?
                    if (auto quitCmd = dynamic_cast<QuitCommand*>(command.get())) {
                        std::cout << "Thanks for playing! Goodbye!\n";
                        return 0;
                    }

                    // Выполнить команду (она обновит состояние игры)
                    controller.executeCommand(command);

                    // Перерисовать поле после команды
                    visualizer.renderField();

                    // Проверить победу игрока
                    if (GameEndHandler::checkPlayerWon(game)) {
                        std::cout << "Congratulations! You cleared level " << game.currentLevel << "!\n";
                        std::cout << "Final score for this level: " << game.player.getScore() << " points\n";

                        // Показать меню улучшений
                        UpgradeSystem::displayUpgradeMenu(game);

                        // Перейти на следующий уровень
                        LevelManager::advanceToNextLevel(game);
                        visualizer.renderField();
                        continue;
                    }
                } catch (const InvalidOperationException& ex) {
                    visualizer.renderMessage(std::string("Input error: ") + ex.what());
                    continue;
                }
            }

            // Обработать конец игры
            GameStateManager stateManager(game);
            if (!GameEndHandler::handleGameEnd(game)) {
                break;
            }
        }
        
        std::cout << "Game ended. Thanks for playing!\n";
        return 0;
        
    } catch (const FileIOException& ex) {
        std::cerr << "File I/O error: " << ex.what() << "\n";
        std::cout << "\nGame will continue with default settings. Please check file permissions.\n";
        return 1;
    } catch (const SaveException& ex) {
        std::cerr << "Save error: " << ex.what() << "\n";
        std::cout << "\nFailed to save game. Your progress may not be persisted.\n";
        return 1;
    } catch (const LoadException& ex) {
        std::cerr << "Load error: " << ex.what() << "\n";
        std::cout << "\nFailed to load save file. Starting new game instead.\n";
        try {
            Game game;
            game.run();
        } catch (const std::exception& nested_ex) {
            std::cerr << "Critical error during new game startup: " << nested_ex.what() << "\n";
            return 2;
        }
    } catch (const ParseException& ex) {
        std::cerr << "Parse error: " << ex.what() << "\n";
        std::cout << "\nSave file is corrupted. Starting new game.\n";
        try {
            Game game;
            game.run();
        } catch (const std::exception& nested_ex) {
            std::cerr << "Critical error during new game startup: " << nested_ex.what() << "\n";
            return 2;
        }
    } catch (const GameLogicException& ex) {
        std::cerr << "Game logic error: " << ex.what() << "\n";
        std::cout << "\nGame initialization failed due to logic error.\n";
        return 1;
    } catch (const InvalidOperationException& ex) {
        std::cerr << "Invalid operation: " << ex.what() << "\n";
        std::cout << "\nGame encountered an invalid state.\n";
        return 1;
    } catch (const std::invalid_argument& ex) {
        std::cerr << "Invalid argument: " << ex.what() << "\n";
        std::cout << "\nGame initialization failed.\n";
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "Unexpected error: " << ex.what() << "\n";
        std::cout << "\nAn unexpected error occurred. Game is terminating.\n";
        return 2;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred\n";
        std::cout << "\nGame is terminating.\n";
        return 3;
    }
    return 0;
}