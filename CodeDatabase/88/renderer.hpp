#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../map/gamemap.hpp"
#include "../entities/player.hpp"
#include "../managers/magic.hpp"
#include "inputhandler.hpp"
#include <iostream>
#include <vector>
#include <memory>

class Game;

class Renderer {
public:
    virtual ~Renderer() = default;
    
    virtual void render(const Game& game) const = 0;
    virtual void renderMessage(const std::string& message) const = 0;
};

class ConsoleRenderer : public Renderer {
public:
    void render(const Game& game) const override;
    
    void renderMessage(const std::string& message) const override {
        std::cout << message << "\n";
    }
};

#endif