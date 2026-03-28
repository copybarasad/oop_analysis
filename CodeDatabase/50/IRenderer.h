#pragma once
#include "Game.h"

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void renderGame(const Game& game) = 0;
    virtual void renderMenu() = 0;
};
