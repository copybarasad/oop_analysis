#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "renderer.h"
#include "game.h"

/**
 * Шаблонный класс визуализации игры
 * @tparam RendererType класс, отвечающий за способ отрисовки
 */
template<typename RendererType>
class GameVisualizer {
private:
    RendererType renderer;  // создает объект параметра шаблона
    
public:
    GameVisualizer() : renderer() {}
    
    // Реагирует на изменения в игре и вызывает отрисовку
    void renderGame(const Game& game) {
        renderer.render(game);
    }
    
    void renderMenu() {
        renderer.renderMenu();
    }

    void renderControls() {
        renderer.renderControls();
    }
    
    void renderGameOver() {
        renderer.renderGameOver();
    }
    
    void clearScreen() {
        renderer.clearScreen();
    }
};

#endif // GAME_VISUALIZER_H