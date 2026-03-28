#include "ConsoleRenderer.h"
#include "Game.h"
#include <iostream>

void ConsoleRenderer::render(const Game& game) {
    
    std::cout << "\n" << game.getFieldString() << std::endl;
    std::cout << game.getPlayerHud() << std::endl;

    const auto& logs = game.getMessageLog();
    if (!logs.empty()) {
        std::cout << "--- События ---" << std::endl;
        for (const auto& msg : logs) {
            std::cout << "> " << msg << std::endl;
        }
    }
    std::cout << "------------------------------------------------" << std::endl;
}
