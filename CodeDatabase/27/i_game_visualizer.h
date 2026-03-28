#pragma once

class Game;

class IGameVisualizer {
public:
    virtual ~IGameVisualizer() = default;
    virtual void Render(const Game& game) = 0;
};
