#include <iostream>
#include <ctime>
#include <cstdlib>

// Подключаем новые заголовочные файлы
#include "world.h"
#include "save_load_manager.h"
#include "console_input.h"
#include "console_renderer.h"
#include "game_visualizer.h"
#include "game_driver.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Используем InputHandler для начального меню (New Game / Load)
    ConsoleInputHandler input;
    int choice = input.GetSaveLoadChoice();
    
    // Создаем объект World здесь (Main владеет миром, Controller только управляет)
    std::unique_ptr<World> world_ptr;

    if (choice == 2) {
        std::string filename = input.GetFilename(false);
        SaveLoadManager manager;
        try {
            // Загрузка
            world_ptr = std::make_unique<World>(manager.LoadGame(filename));
            std::cout << "World loaded.\n";
        } catch (const std::exception& e) {
            std::cout << "Load failed: " << e.what() << ". Starting new game.\n";
            world_ptr = std::make_unique<World>(12, 12);
        }
    } else {
        // Новая игра
        world_ptr = std::make_unique<World>(12, 12);
    }

    // Инициализация шаблонных классов
    // GameDriver управляет конкретным World, используя ConsoleInputHandler и GameVisualizer<ConsoleRenderer>
    GameDriver<ConsoleInputHandler, GameVisualizer<ConsoleRenderer>> engine(*world_ptr);
    
    // Запуск цикла
    engine.Run();

    return 0;
}