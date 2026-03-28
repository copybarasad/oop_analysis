#include "game.h"
#include "console_input.h"
#include "console_renderer.h"
#include "game_visualizer.h"
#include "game_manager.h"
// #include "serial_input.h" 
#include <cstring>
#include "web_renderer.h"

void setupGame(Game& game) {
    Prints::print_menu_options();
    std::string choice;
    std::cin >> choice;

    try {
        while (true) {
            if (choice == "load") {
                game.loadGame("savefile.txt");
                break;
            } else if (choice == "start") {
                game.startNewGame();
                break;
            } else {
                Prints::print_incorrect_input_string();
                std::cin >> choice;
            }
        }
    } catch (const std::runtime_error& e) {
        Prints::print_load_game_error(e);
        Prints::print_starting_new_game();
        game.startNewGame();
    }
}

int main(int argc, char* argv[]) {

    bool useFileLogging = false;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (std::strcmp(argv[i], "--log-file") == 0) {
                useFileLogging = true;
            }
        }
    }

    bool useWebRender = false;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (std::strcmp(argv[i], "--web-render") == 0) {
                useWebRender = true;
            }
        }
    }

    // bool useArduino = false;
    // for (int i = 1; i < argc; ++i) {
    //     if (std::strcmp(argv[i], "--arduino") == 0) useArduino = true;
    // }

    Prints::init(useFileLogging);

    if (useFileLogging) {
        std::cout << "Логирование включено в файл 'game_log.txt'. В консоли будет пусто." << std::endl;
    }

    Game game;

    setupGame(game);

// if (useArduino) {
//         std::cout << "=== РЕЖИМ ARDUINO КОНТРОЛЛЕРА ===" << std::endl;
//         std::cout << "Убедитесь, что Arduino подключена к порту COM... (Win) или /dev/tty... (Linux)" << std::endl;

//         GameManager<SerialInput> arduinoManager;
//         GameVisualizer<ConsoleRenderer> visualizer;

//         arduinoManager.run(game, visualizer);

//     } else {
//         std::cout << "=== ОБЫЧНЫЙ РЕЖИМ ===" << std::endl;

        GameManager<ConsoleInput> consoleManager;
        GameVisualizer<ConsoleRenderer> visualizeCon;
        GameVisualizer<WebRenderer> visualizerWeb;
        if (useWebRender)
            consoleManager.run(game, visualizerWeb);
        else {
            consoleManager.run(game, visualizeCon);
        }
//     }

    return 0;
}