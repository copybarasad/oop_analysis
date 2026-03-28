#include <iostream>
#include <ctime>
#include "game.h"
#include "save_load_manager.h"
#include "game_controller.h"
#include "game_view.h"
#include "input_handler.h"  
#include "renderer.h"  
#include "logger.h"  
#include "event_system.h"  


#ifdef _WIN32
#include <windows.h>
#endif

void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void display_main_menu() {
    std::cout << "=== МАГИЧЕСКАЯ БИТВА ===\n";
    std::cout << "1. Новая игра\n";
    std::cout << "2. Загрузить игру\n";
    std::cout << "3. Настройки управления\n";
    std::cout << "4. Выйти\n";
    std::cout << "Выбор: ";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::srand(std::time(0));

    try {

        Logger::init("game.log", Log_output::BOTH);
        Logger::get_instance().info("Программа запущена");

        Game game;
        bool program_running = true;
        
        while (program_running) {
            clear_console();
            display_main_menu();
            
            int choice;
            std::cin >> choice;
            
            switch (choice) {
                case 1: {
                    clear_console();
                    std::cout << "=== НОВАЯ ИГРА ===\n";

                    Logger::get_instance().info("Начата новая игра"); 
                    
                    game.start_new_game_external();
                    
                    Game_controller<Console_input_handler> controller(&game);
                    Game_view<Console_renderer> view(&game);
                    
                    if (!controller.get_input_handler().load_bindings_from_file("controls.txt")) {
                        std::cout << "Используются стандартные настройки управления\n";
                    }
                    
                    std::cout << "Игра началась! Используйте WASD для движения, Z для выхода.\n";
                    std::cout << "Нажмите Enter чтобы начать...";
                    std::cin.ignore();
                    std::cin.get();
                    
                    while (game.is_running() && game.get_player() && game.get_player()->is_alive()) {
                        view.render();
                        controller.process_input();
                    }
                    
                    Logger::get_instance().info("Игра завершена"); 
                    std::cout << "Нажмите Enter чтобы вернуться в меню...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                
                case 2: {
                    clear_console();
                    std::cout << "=== ЗАГРУЗКА ИГРЫ ===\n";
                    
                    std::cout << "Введите имя файла сохранения: ";
                    std::string filename;
                    std::cin >> filename;
                    
                    if (game.load_game(filename)) {
                        Logger::get_instance().info("Игра загружена из файла: " + filename);

                        std::cout << "Игра загружена успешно!\n";
                        
                        game.start_external();
                        
                        Game_controller<Console_input_handler> controller(&game);
                        Game_view<Console_renderer> view(&game);
                        
                        if (!controller.get_input_handler().load_bindings_from_file("controls.txt")) {
                            std::cout << "Используются стандартные настройки управления\n";
                        }
                        
                        std::cout << "Нажмите Enter чтобы продолжить...";
                        std::cin.ignore();
                        std::cin.get();
                        
                        while (game.is_running() && game.get_player() && game.get_player()->is_alive()) {
                            view.render();
                            controller.process_input();
                        }
                        
                    } else {
                        Logger::get_instance().error("Ошибка загрузки игры из файла: " + filename); 
                        std::cout << "Ошибка загрузки!\n";
                        std::cout << "Нажмите Enter чтобы вернуться в меню...";
                        std::cin.ignore();
                        std::cin.get();
                    }
                    break;
                }
                
                case 3: {
                    clear_console();
                    std::cout << "=== НАСТРОЙКИ УПРАВЛЕНИЯ ===\n";
                    std::cout << "Текущие настройки (файл controls.txt):\n";
                    std::cout << "w - движение вверх\n";
                    std::cout << "s - движение вниз\n";
                    std::cout << "a - движение влево\n";
                    std::cout << "d - движение вправо\n";
                    std::cout << "e - атака\n";
                    std::cout << "q - смена режима боя\n";
                    std::cout << "1-5 - заклинания\n";
                    std::cout << "c - сохранить\n";
                    std::cout << "j - загрузить\n";
                    std::cout << "h - лечение\n";
                    std::cout << "n - выучить заклинание\n";
                    std::cout << "z - выход\n";
                    std::cout << "\nДля изменения отредактируйте файл controls.txt\n";
                    std::cout << "Нажмите Enter чтобы вернуться...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                
                case 4: {
                    program_running = false;
                    Logger::get_instance().info("Программа завершена");
                    std::cout << "До свидания!\n";
                    break;
                }
                
                default: {
                    std::cout << "Неверный выбор!\n";
                    std::cout << "Нажмите Enter чтобы продолжить...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
            }
        }
    } catch (const std::exception& e) {
        Logger::get_instance().error("Критическая ошибка: " + std::string(e.what()));
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        std::cout << "Нажмите Enter для выхода...";
        std::cin.ignore();
        std::cin.get();
        return 1;
    }
    return 0;
}
