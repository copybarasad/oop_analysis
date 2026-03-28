#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include "GameEngine.h"
#include <string>

class ConsoleRenderer {
public:
    void render(const GameEngine& game, int currentLevel) const;
    
private:
    void renderGameField(const GameEngine& game) const;
    void renderGameInfo(const GameEngine& game, int currentLevel) const;
    void renderEnemiesInfo(const GameEngine& game) const;
    void renderTrapsInfo(const GameEngine& game) const;
    void renderControls() const;
    void renderLegend() const;
    std::string getCellSymbol(const GameEngine& game, const Position& pos) const;
};

#endif