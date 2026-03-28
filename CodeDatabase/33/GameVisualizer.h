#pragma once
#include "GameEventBus.h"
#include "Game.h"
#include <utility>

template <typename Renderer>
class GameVisualizer : public GameEventBus::Listener {
public:
    GameVisualizer(Renderer renderer, GameEventBus* bus);
    ~GameVisualizer() override;

    void OnEvent(const GameEvent&) override;
    void RenderIfNeeded(const Game& game);
    void ForceRender(const Game& game);

private:
    Renderer renderer_;
    GameEventBus* bus_;
    bool needs_render_ = true;
};
