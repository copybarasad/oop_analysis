#pragma once
#include "renderer_interface.h"
#include "game_session.h"
#include <type_traits>
#include <memory>

class ConsoleRenderer;

template<typename RendererType>
class GameVisualizer {

public:
    GameVisualizer(std::unique_ptr<RendererType> renderer);
    
    void VisualizeWelcome();
    void VisualizeLevelStart(int level, int enemy_count);
    void VisualizeGameState(const GameSession& game);
    void VisualizeGameResult(const GameSession& game);
    
private:
    std::unique_ptr<RendererType> renderer_;
};
