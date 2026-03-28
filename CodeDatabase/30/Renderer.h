#pragma once
#include "GameCycle.h"

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const GameCycle& game) = 0;
};
