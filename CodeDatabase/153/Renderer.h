#ifndef RENDERER_H
#define RENDERER_H

#include <string>

class GameState;

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const GameState& gameState) const = 0; 
    virtual void showMessage(const std::string& message) const = 0;
};

#endif