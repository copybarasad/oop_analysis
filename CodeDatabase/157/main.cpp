#include "GameModel.h"
#include "GameController.h"
#include "Visualizer.h"
#include "ConsoleInput.h"
#include "ConsoleRenderer.h"
#include "SaveLoadException.h"
#include <iostream>
#include <string>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#endif
int main() {
    std::srand(std::time(nullptr));
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif

    GameModel gm;
    std::cout << "=== START ===\n";
    std::cout << "Press Enter for new game, or type filename to load: ";
    std::string s;
    std::getline(std::cin, s);
    if (!s.empty()) {
        try { gm.loadFromFile(s); std::cout << "loaded " << s << "\n"; }
        catch (const SaveLoadException &e) { std::cout << "load failed: " << e.what() << ", starting new\n"; }
    } else {
        std::cout << "new game\n";
    }

    Visualizer<ConsoleRenderer> vis;
    GameController<ConsoleInput> ctrl(gm);

    vis.show(gm);
    while (true) {
        std::cout << "> ";
        if (!ctrl.processOnce()) break;
        vis.show(gm);
        if (gm.playerHp() <= 0) {
            std::cout << "you died\n";
            break;
        }
    }
    return 0;
}
