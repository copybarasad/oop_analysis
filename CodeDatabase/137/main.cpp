#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameRenderer.h"
#include "InputReader.h"
#include "KeyBindings.h"
#include "GameController.h"
#include "GameView.h"
#include "Logger.h"
#include "GameException.h"
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {
    bool log_to_file = false;
    bool log_to_console = false;
    std::string log_filename = "game.log";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--log-file" || arg == "-lf") {
            log_to_file = true;
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                log_filename = argv[++i];
            }
        } else if (arg == "--log-console" || arg == "-lc") {
            log_to_console = true;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options]\n"
                      << "Options:\n"
                      << "  --log-file, -lf [filename]  Enable logging to file (default: game.log)\n"
                      << "  --log-console, -lc          Enable logging to console\n"
                      << "  --help, -h                  Show this help message\n";
            return 0;
        }
    }

    sf::RenderWindow window(sf::VideoMode({1300, 900}), "Game");
    window.setFramerateLimit(60);

    Game game;

    Logger logger;
    if (log_to_file) {
        try {
            logger.AddWriter(std::make_unique<FileLogWriter>(log_filename));
            std::cout << "Logging to file: " << log_filename << std::endl;
        } catch (const FileOpenException& e) {
            std::cerr << "Warning: Could not open log file: " << e.what() << std::endl;
        }
    }
    if (log_to_console) {
        logger.AddWriter(std::make_unique<ConsoleLogWriter>());
        std::cout << "Logging to console enabled" << std::endl;
    }

    LoggerObserver log_observer(logger);
    if (log_to_file || log_to_console) {
        game.AddEventObserver(&log_observer);
    }

    KeyBindings bindings;
    try {
        bindings.LoadFromFile("config/keybindings.cfg");
        std::cout << "Key bindings loaded from config/keybindings.cfg" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Using default key bindings" << std::endl;
    }

    SfmlGameRenderer renderer(window);
    SfmlInputReader input_reader(window, bindings);

    GameController<SfmlInputReader> controller(input_reader, game);
    GameView<SfmlGameRenderer> view(renderer, game);

    controller.SetSaveFilename("savegame.sav");

    while (window.isOpen() && controller.IsRunning()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                GameState state = game.GetState();

                if (state == GameState::PAUSE_MENU && key_pressed->code == sf::Keyboard::Key::Escape) {
                    game.ResumeGame();
                    continue;
                }

                if (state == GameState::PAUSE_MENU && key_pressed->code == sf::Keyboard::Key::Q) {
                    controller.Stop();
                    continue;
                }

                if (state == GameState::PAUSE_MENU && key_pressed->code == sf::Keyboard::Key::S) {
                    try {
                        game.SaveGame("savegame.sav");
                        std::cout << "Game saved successfully!" << std::endl;
                    } catch (const GameException& e) {
                        std::cerr << "Failed to save game: " << e.what() << std::endl;
                    }
                    continue;
                }

                if (state == GameState::PAUSE_MENU && key_pressed->code == sf::Keyboard::Key::R) {
                    game.StartNewGame();
                    continue;
                }

                if ((state == GameState::GAME_OVER || state == GameState::MENU) &&
                    key_pressed->code == sf::Keyboard::Key::Escape) {
                    controller.Stop();
                    continue;
                }

                if (key_pressed->code == sf::Keyboard::Key::L &&
                    (state == GameState::MENU || state == GameState::PAUSE_MENU || 
                     state == GameState::GAME_OVER)) {
                    try {
                        game.LoadGame("savegame.sav");
                        std::cout << "Game loaded successfully!" << std::endl;
                    } catch (const GameException& e) {
                        std::cerr << "Failed to load game: " << e.what() << std::endl;
                    }
                    continue;
                }

                input_reader.ProcessEvent(*event);
            }

            if (const auto* mouse_pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse_pressed->button == sf::Mouse::Button::Left) {
                    view.HandleClick(mouse_pressed->position.x, mouse_pressed->position.y);
                }
            }
        }

        controller.ProcessInput();

        view.Update();
    }

    if (log_to_file || log_to_console) {
        game.RemoveEventObserver(&log_observer);
    }

    return 0;
}
