#include "ConsoleView.h"
#include <iostream>

void ConsoleView::show_message(const std::string& msg) {
    std::cout << "[GAME]: " << msg << "\n";
}

void ConsoleView::on_level_init(int level) {
    std::cout << "\n=== Level " << level << " Initialized ===\n";
}

void ConsoleView::on_game_over(bool win) {
    if (win) std::cout << "\n*** VICTORY! ***\n";
    else std::cout << "\n=== GAME OVER ===\n";
}

int ConsoleView::ask_upgrade_choice() {
    std::cout << "\n=== Level Up! ===\n";
    std::cout << "Choose upgrade:\n";
    std::cout << "1. Increase Health (+20)\n";
    std::cout << "2. Increase Damage (+10)\n";
    std::cout << "3. Add Spell Slot\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;
    return choice;
}

int ConsoleView::ask_game_over_action() {
    std::cout << "What to do?\n";
    std::cout << "1. Restart\n2. Load Save\n3. Exit\n> ";

    int choice;
    std::cin >> choice;
    return choice;
}
