#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "GameController.h"

template<typename TRenderer>
class GameVisualizer {
private:
    TRenderer renderer;
    GameController* gameController;
    
public:
    GameVisualizer(GameController* controller)
        : renderer(), gameController(controller) {}
    
    void render() {
        if (!gameController) return;
        
        const auto& gameManager = gameController->getGameManager();
        int currentLevel = gameController->getCurrentLevel();
        renderer.render(gameManager, currentLevel);
    }
};

#endif
