#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include <memory>

class Game;
class GameMenu;

// ШАБЛОННЫЙ класс - параметр RendererType может быть ЛЮБЫМ классом
template <typename RendererType>
class GameVisualizer {
public:
    GameVisualizer(Game& game, std::unique_ptr<RendererType> renderer)
        : game_(game), renderer_(std::move(renderer)) {}
    
    void renderGameState() {
        renderer_->renderGame(game_);
    }
    
    void renderGameEnd() {
        renderer_->renderGameEnd(game_.getGameEndData());
    }
    
    void showMessage(const std::string& message) {
        renderer_->renderMessage(message);
    }
    
    void clear() {
        renderer_->clearScreen();
    }
    
private:
    Game& game_;
    std::unique_ptr<RendererType> renderer_;
};

#endif