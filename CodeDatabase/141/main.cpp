#include "MenuController.h"
#include <iostream>

int main(int argc, char **argv) {
    try {
        MenuController menu(argc, argv);
        menu.run();
    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
