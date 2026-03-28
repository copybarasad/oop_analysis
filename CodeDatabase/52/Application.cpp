#include "Application.h"
#include "Game.h"
#include "GameControllerT.h"
#include "ConsoleInput.h"
#include <iostream>

Application::Application() {
    menu_ = std::make_unique<MenuController>();
}

void Application::Run() {
    setlocale(LC_ALL, "Russian");
    int game_flag = 1;

    while (game_flag) {
        int choice = menu_->ShowMainMenu();
        if (choice == -1)
            continue;

        if (choice == 3) {
            std::cout << "Выход из программы.\n";
            return;
        }

        Game game;

        if (choice == 2) {
            try {
                SaveManager::Load(game, "saves/save1.txt");
                std::cout << "Игра загружена.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Ошибка загрузки: " << e.what() << "\n";
                std::cout << "Начата новая игра.\n";
            }
        }

        GameControllerT<ConsoleInput> controller(game);
        controller.Run();

        // меню после игры
        while (game_flag) {
            int endChoice = menu_->ShowGameOverMenu();

            if (endChoice == -1)
                continue;
            else if (endChoice == 1)
                break;          // новая игра
            else if (endChoice == 2)
                break;          // в главное меню
        }
    }
}
