#include "GameManager.h"
#include <iostream>
#include <exception>

int main() {
    try {
        GameManager gameManager;
        gameManager.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 1;
    }
    return 0;
}