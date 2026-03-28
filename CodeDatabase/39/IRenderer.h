#pragma once

class Game;

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void Render(Game& game) = 0;
};
