#ifndef RENDERER_H
#define RENDERER_H

#include <string>

// Предварительное объявление
class Game;

// Абстрактный интерфейс отрисовки
class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const Game& game) = 0;
    virtual void renderMenu() = 0;
    virtual void renderGameOver() = 0;
    virtual void renderControls() = 0;
};

#endif