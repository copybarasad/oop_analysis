#include <iostream>
#include <windows.h>
#include "Game.h"
#include "GameData.h"
#include "Loader.h"
#include "ConsoleInputHandler.h"
#include "ConsoleRenderer.h"
#include "GameManager.h"
#include "Visualizer.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    bool user_exited = false;
    while (!user_exited) {
        Loader loader("loads");
        GameData game_data;
        bool loadSuccess = false;

        if (loader.doSavesExist()) {
            std::cout << "1. Новая игра\n2. Загрузить сохранение\n3. Выход\nВыбор: ";
            char choice;
            std::cin >> choice;
            if (choice == '3') {
                user_exited = true;
                continue;
            }
            if (choice == '2') {
                std::cout << "Слот (1-5): ";
                int slot;
                std::cin >> slot;
                if (loader.loadGame(slot, game_data)) {
                    loadSuccess = true;
                    std::cout << "Загружено!\n";
                }
            }
            std::cin.ignore(1000, '\n');
        }

        if (!loadSuccess) game_data = GameData(true);

        Game game(game_data, loader);
        GameManager<ConsoleInputHandler> gameManager(game);
        Visualizer<ConsoleRenderer> visualizer;

        while (game.isRunning()) {
            visualizer.draw(game);
            gameManager.update();
        }

        visualizer.draw(game);
        std::cout << "Game Over." << std::endl;
    }
    return 0;
}
