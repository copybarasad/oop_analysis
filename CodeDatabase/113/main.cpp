#include "Game.h"
#include <iostream>
#include <exception>
#include <crtdbg.h>

// Глобальный обработчик необработанных исключений
void TerminateHandler() {
    std::cerr << "Terminate called due to unhandled exception!" << std::endl;
    exit(1);
}

int main() {
    // Устанавливаем обработчики
    std::set_terminate(TerminateHandler);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cout << "=== GAME LAUNCH ===" << std::endl;

    try {
        // Используем аргументы по умолчанию для шаблона
        Game<> game;  // Это эквивалентно Game<CommandInputHandler, GameRenderer>
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception in main!" << std::endl;
        return 1;
    }

    std::cout << "=== GAME EXITED NORMALLY ===" << std::endl;
    return 0;
}