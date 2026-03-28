#include "MainMenu.h"
#include "ExceptionHandler.h"
#include <iostream>

int main() {
    try {
        std::cout << "========================================\n";
        std::cout << "      HERO ADVENTURE GAME\n";
        std::cout << "  Laboratory Works 1-4 Combined\n";
        std::cout << "========================================\n\n";
        
        MainMenu menu;
        menu.run();
    } 
    catch (const std::exception& e) {
        std::cerr << "\n=== UNHANDLED EXCEPTION ===\n";
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << "The game has crashed unexpectedly.\n";
        std::cerr << "Please report this issue.\n";
        std::cerr << "=============================\n";
        
        // Даем пользователю время прочитать сообщение
        std::cout << "\nPress Enter to exit...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        
        return 1;
    }
    catch (...) {
        std::cerr << "\n=== UNKNOWN ERROR ===\n";
        std::cerr << "The game has crashed due to an unknown error.\n";
        std::cerr << "========================\n";
        
        std::cout << "\nPress Enter to exit...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        
        return 1;
    }
    
    return 0;
}