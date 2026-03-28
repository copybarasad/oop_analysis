#include "game_visualizer.h"
#include "console_renderer.h"

template<typename RendererType>
GameVisualizer<RendererType>::GameVisualizer(std::unique_ptr<RendererType> renderer)
    : renderer_(std::move(renderer)) {}

template<typename RendererType>
void GameVisualizer<RendererType>::VisualizeWelcome() {
    renderer_->RenderWelcome();
}

template<typename RendererType>
void GameVisualizer<RendererType>::VisualizeLevelStart(int level, int enemy_count) {
    renderer_->RenderLevelStart(level, enemy_count);
}

template<typename RendererType>
void GameVisualizer<RendererType>::VisualizeGameState(const GameSession& game) {
    renderer_->RenderField(game.GetGameField(), game.GetPlayer(), game.GetEnemyManager());
    renderer_->RenderStatus(game.GetPlayer(), game.GetEnemyManager());
}

template<typename RendererType>
void GameVisualizer<RendererType>::VisualizeGameResult(const GameSession& game) {
    renderer_->RenderGameResult(game.GetGameField(), game.GetPlayer(), game.GetEnemyManager());
}

template class GameVisualizer<ConsoleRenderer>;
