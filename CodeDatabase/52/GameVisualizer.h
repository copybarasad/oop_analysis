
#pragma once
#include "Game.h"
#include "CommandBindings.h"

template <class TRenderer>
class GameVisualizer {
public:
    GameVisualizer() = default;

    void Draw(const Game& game) {
        renderer_.RenderField(game.GetField());
        renderer_.RenderHUD(game);
    }
    void ShowMessage(const char* msg) {
        renderer_.RenderMessage(msg);
    }
    void SetBindings(const CommandBindings& b) {
        renderer_.SetBindings(b);
    }
private:
    TRenderer renderer_;
};
