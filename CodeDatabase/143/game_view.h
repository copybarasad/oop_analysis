#pragma once

#include "game_controller.h"
#include "console_render.h"

template<typename Renderer = ConsoleRenderer>
class GameView {
public:
    explicit GameView(const GameController& ctrl) : controller(ctrl) {}

    void update() const {
        renderer.render(controller);
    }

    void showMessage(const std::string& msg) const {
        renderer.renderMessage(msg);
    }

private:
    const GameController& controller;
    Renderer renderer;
};
