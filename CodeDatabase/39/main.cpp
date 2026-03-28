#include "Game.h"
#include "ConsoleInputReader.h"
#include "ConsoleRenderer.h"
#include "GameController.h"
#include "GameView.h"
#include "KeyBindings.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "LoggingObserver.h"

#include <iostream>
#include <cctype>
#include <memory>
#include <string>

int main(int argc, char** argv) {
    std::cout << "Load game (L) or start new (N)? ";
    char c;
    if (!(std::cin >> c)) {
        return 0;
    }
    c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));

    std::unique_ptr<Game> game_ptr;

    if (c == 'L') {
        try {
            std::size_t w = 15, h = 15;
            game_ptr = std::make_unique<Game>(w, h);
            game_ptr->LoadFromFile("save.txt");
        } catch (const std::exception& e) {
            std::cout << "Load error: " << e.what() << "\n";
            std::cout << "Starting new game instead.\n";

            std::size_t w, h;
            std::cout << "Enter field width (10-25): ";
            std::cin >> w;
            std::cout << "Enter field height (10-25): ";
            std::cin >> h;

            game_ptr = std::make_unique<Game>(w, h);
        }
    } else {
        std::size_t w, h;
        std::cout << "Enter field width (10-25): ";
        std::cin >> w;
        std::cout << "Enter field height (10-25): ";
        std::cin >> h;

        game_ptr = std::make_unique<Game>(w, h);
    }

    Game& game = *game_ptr;

    // --- выбор логгера по аргументам ---
    std::shared_ptr<ILogger> logger;
    if (argc > 1 && std::string(argv[1]) == "--log-file") {
        std::string filename = (argc > 2) ? argv[2] : "game.log";
        logger = std::make_shared<FileLogger>(filename);
    } else {
        logger = std::make_shared<ConsoleLogger>();
    }

    LoggingObserver log_observer(logger);
    game.AddObserver(&log_observer);

    // загружаем раскладку клавиш
    KeyBindings bindings;
    bindings.LoadFromFile("keys.cfg");

    ConsoleInputReader input(bindings);
    GameView<ConsoleRenderer> view;
    GameController<ConsoleInputReader> controller(game, input);

    view.Render(game);
    controller.Run(view);

    return 0;
}
