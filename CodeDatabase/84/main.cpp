#include "game.h"
#include "input_handler.h"
#include "console_renderer.h"
#include "game_visualizer.h"
#include "game_manager.h"
#include <iostream>

int main() {
    try {
        Game game(20, 10);
        GameManager<InputHandler, GameVisualizer<ConsoleRenderer>> manager(game);
        manager.run();
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}