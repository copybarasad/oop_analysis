#pragma once

#include "GameRenderer.h"
#include "GameController.h"

class Game;

// Шаблонный класс визуализации. Создаёт конкретный renderer и вызывает его.
template <typename Renderer>
class GameVisualizer {
public:
    GameVisualizer() = default;

    void render(const Game& game, const GameController& controller) {
        renderer.render(game, controller);
    }

private:
    Renderer renderer;
};
