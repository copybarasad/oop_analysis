#ifndef RENDERER_H
#define RENDERER_H

#include <string>

class Game; 

// Interface for renderers
class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const Game& game) = 0;
    virtual void showMessage(const std::string& message) = 0;
};

#endif