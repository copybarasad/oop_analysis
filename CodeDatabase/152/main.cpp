#define NOMINMAX
#include <Windows.h>
#include <locale>

#include "game.h"
#include "input_reader.h"
#include "renderer.h"
#include "game_controller.h"
#include "game_view.h"

int main() {

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "");

    Game game;

    GameController<ConsoleInputReader> controller(game);
    GameView<ConsoleRenderer> view(game);


    game.run();


    while (true) {


        view.update();

        controller.loop();

        if (game.check_victory_condition()) {
            std::cout << "\n=== Уровень пройден! ===\n";
            std::cout << "Начать новый уровень? (y/n): ";
            char c;
            std::cin >> c;

            if (c == 'y' || c == 'Y') {
                game.start_level();
                continue;
            }
            else {
                std::cout << "Игра завершена." << std::endl;
                break;
            }
        }

        if (game.check_defeat_condition()) {
            std::cout << "\n=== Вы погибли! ===\n";
            std::cout << "Начать заново? (y/n): ";
            char c;
            std::cin >> c;

            if (c == 'y' || c == 'Y') {
                game.start_level();
                continue;
            }
            else {
                std::cout << "Игра завершена." << std::endl;
                break;
            }
        }
    }

    return 0;
}
