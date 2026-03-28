#include "game.h"
#include "console_input.h"
#include "console_renderer.h"
#include "game_visualizer.h"
#include "game_manager.h"
#include <cstring>
#include "log.h"

void setupGame(Game& game) {
    Prints::print_menu_options();
    ConsoleInput input;
    GameCommand cmd = input.getCommand();
    try {
        switch (cmd.type)
        {
        case CommandType::Load:{
            game.loadGame("savefile.txt");
                break;
        }
        case CommandType::Start:{
            game.startNewGame();
                break;
        }
        default:
            Prints::print_incorrect_input_string();
            cmd = input.getCommand();
            break;
        } 
    }catch (const FileOpenError& e ) {
        Prints::print_load_game_error(e);
        Prints::print_starting_new_game();
        game.startNewGame();
    } catch (const SaveDataError& e){
        Prints::print_load_game_error(e);
        Prints::print_starting_new_game();
        game.startNewGame();
    } catch (const std::runtime_error& e) {
        Prints::print_load_game_error(e);
        Prints::print_starting_new_game();
        game.startNewGame();
    }
}

int main(int argc, char* argv[]) {

    bool logToConsole = true; // По умолчанию пишем в консоль
    bool logToFile = false;   // По умолчанию не пишем в файл

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (std::strcmp(argv[i], "--log-file") == 0) {
                // Только файл
                logToConsole = false;
                logToFile = true;
            }
            else if (std::strcmp(argv[i], "--log-both") == 0) {
                // И файл, и консоль
                logToConsole = true;
                logToFile = true;
            }
            else if (std::strcmp(argv[i], "--log-none") == 0) {
                // Никуда
                logToConsole = false;
                logToFile = false;
            }
        }
    }

    Log::init(logToConsole, logToFile);

    if (logToFile && !logToConsole) {
        std::cout << "Логирование включено в файл 'game_log.txt'. В консоли будет пусто." << std::endl;
    } else if (logToFile && logToConsole) {
        std::cout << "Логирование дублируется: вывод идет и в консоль, и в файл 'game_log.txt'." << std::endl;
    } else if (!logToFile && !logToConsole) {
        std::cout << "Логирование отключено." << std::endl;
    }

    Game game;

    setupGame(game);

    GameManager<ConsoleInput> consoleManager;
    GameVisualizer<ConsoleRenderer> visualizeCon;

    consoleManager.run(game, visualizeCon);
    
    Log::close();

    return 0;
}