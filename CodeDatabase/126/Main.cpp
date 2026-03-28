#include "../headers/ui/MenuManager.h"

int main(){
    try {
        MenuManager menu;
        menu.handleMainMenu();
    }
    catch (const std::exception& e) {
        std::cout << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}