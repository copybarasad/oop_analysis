#ifndef RENDERING_H
#define RENDERING_H

#include "Game.h"
#include <iostream>

class IGameRenderer {
public:
    virtual void render(Game& game) = 0;
    virtual ~IGameRenderer() = default;
};

class ConsoleRenderer : public IGameRenderer {
public:
    void render(Game& game) override; 
};

#endif