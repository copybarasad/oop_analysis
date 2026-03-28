#include "menu_controller.hpp"
#include "process_game.hpp"
#include <iostream>

int main() {
    try {
        MenuController menuController;
        menuController.runMainMenu();
        std::cout << "\nСпасибо за игру!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Исключение в main: " << e.what() << std::endl;
        return 1;
    }
}