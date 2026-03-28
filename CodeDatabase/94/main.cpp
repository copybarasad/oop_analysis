#include "Game.h"
#include "GameExecutor.h"
#include "GameManager.h"
#include "GameVisualizer.h"
#include "Renderer.h"       
#include "InputHandler.h"   
#include <iostream>

int main() {
    try {
        // Создание экземпляра игры
        Game game;
        
        // Создание исполнителя команд
        GameExecutor executor(game);
        
        // Создание визуализатора игры с консольным рендерером
        GameVisualizer<ConsoleRenderer> visualizer(game);
        
        // Создание менеджера игры с консольным обработчиком ввода
        GameManager<ConsoleInputHandler> manager(
            executor, 
            game, 
            ConsoleInputHandler(game)
        );
        
        // Начальная отрисовка
        visualizer.forceRender();
        
        // Основной игровой цикл
        while (game.isRunning() && manager.isRunning()) {
            if (manager.process()) {
                visualizer.render();
            }
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Фатальная ошибка: " << e.what() << std::endl;
        std::cerr << "Нажмите любую клавишу для выхода..." << std::endl;
        std::cin.get();
        return 1;
    }
}