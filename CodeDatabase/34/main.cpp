#include "App.h"
#include "utils/RandomGenerator.h" 
#include <iostream>
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    Utils::RandomGenerator::getInstance().seed();

    try {
        Game::App app(argc, argv);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "A critical, unrecoverable error occurred: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\nProgram finished. Press Enter to close the window...";
    std::cin.get();

    return 0;
}