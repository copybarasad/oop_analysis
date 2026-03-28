#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "LevelController.h"
#include <string>
#include <iostream>

template<typename TRenderer>
class GameVisualizer {
private:
    TRenderer& renderer;
    
public:
    explicit GameVisualizer(TRenderer& rendererRef)
        : renderer(rendererRef) {}
    
    void renderFullGame(const LevelController& level, int turnCount) {
        renderer.renderGameState(level, turnCount);
    }
    
    void renderMessage(const std::string& message) {
        renderer.renderMessage(message);
    }
    
    void renderGameOver(bool victory, const Player& player) {
        renderer.renderGameOver(victory, player);
    }
    
    void renderCommands() {
        renderer.renderCommands();
    }
};

#endif