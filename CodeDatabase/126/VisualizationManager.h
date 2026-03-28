#ifndef VISUALIZATIONMANAGER_H
#define VISUALIZATIONMANAGER_H

#include "../gamelogic/Game.h"
#include "../render/GameRenderer.h"

template<typename Renderer>
class VisualizationManager {
    Renderer renderer;
    
public:
    VisualizationManager() = default;
    
    void renderGame(Game& game) {
        renderer.renderHeader(
            game.getField(), 
            game.getPlayer(), 
            game.getCurrentLevel(),
            game.getEnemies(), 
            game.getTowers(), 
            game.getTraps()
        );
        renderer.renderField(game.getField());
    }
    
    void renderGameOver(GameStatus status, int currentLevel, int score) {
        renderer.renderGameOver(status, currentLevel, score);
    }
};

#endif