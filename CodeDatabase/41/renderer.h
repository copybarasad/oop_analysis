#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"
#include "game_field.h"
#include "player.h"
#include <string>
#include <iostream>

class Renderer{
public:
    virtual ~Renderer() = default;
    virtual void render(const Game& game) = 0;
    virtual void display_message(const std::string& message) = 0;
    virtual void clear_screen() = 0;
};

class Console_renderer : public Renderer{
public:
    void render(const Game& game) override;
    void display_message(const std::string& message) override;
    void clear_screen() override;
};

#endif