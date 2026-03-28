#pragma once
#include "GameModel.h"

// Интерфейс для отрисовки игры
class Renderer {
public:
    virtual ~Renderer() = default;
    
    // Отрисовывает игру
    virtual void render(float dt) = 0;
};

