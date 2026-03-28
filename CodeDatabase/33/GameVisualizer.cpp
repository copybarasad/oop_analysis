#include "GameVisualizer.h"
#include "ConsoleRenderer.h"
template class GameVisualizer<ConsoleRenderer>;

template <typename Renderer>
GameVisualizer<Renderer>::GameVisualizer(Renderer renderer, GameEventBus* bus)
    : renderer_(std::move(renderer)), bus_(bus) {
    if (bus_) bus_->Subscribe(this);
}

template <typename Renderer>
GameVisualizer<Renderer>::~GameVisualizer() {
    if (bus_) bus_->Unsubscribe(this);
}

template <typename Renderer>
void GameVisualizer<Renderer>::OnEvent(const GameEvent&) {
    needs_render_ = true;
}

template <typename Renderer>
void GameVisualizer<Renderer>::RenderIfNeeded(const Game& game) {
    if (needs_render_) {
        renderer_.Render(game);
        needs_render_ = false;
    }
}

template <typename Renderer>
void GameVisualizer<Renderer>::ForceRender(const Game& game) {
    renderer_.Render(game);
    needs_render_ = false;
}
