#ifndef RENDERER_H_
#define RENDERER_H_

#include "field.h"
#include "player.h"
#include <iostream>

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;
    
    void RenderField(const Field& field) const;
    void RenderPlayerInfo(const Player& player) const;
    void RenderHand(const Hand& hand) const;
    void RenderGameState(const Field& field) const;
    void RenderMessage(const std::string& message) const;
    void RenderHelp() const;
    
private:
    char CellToChar(CellType type) const;
    void RenderSeparator(int width) const;
};

#endif // RENDERER_H_
