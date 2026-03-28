#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Game.h"
#include "GameVisualizer.h"

template <typename InputHandler>
class GameManager {
private:
    InputHandler inputHandler;

public:
    template <typename Renderer>
    void run(Game& game, GameVisualizer<Renderer>& visualizer) {
        while (game.getIsGameActive()) {
            visualizer.draw(game);
            
            bool turnCompleted = false;
            while (!turnCompleted && game.getIsGameActive()) {
                auto command = inputHandler.getCommand(game);
                
                turnCompleted = game.executeCommand(command);
            }
            
            if (turnCompleted && game.getIsGameActive()) {
                game.handleEnemyTurns();
                game.refreshGameState();
                game.checkForNewSpell();
                game.checkGameCompletion();
                game.setCurrentTurn(game.getCurrentTurn() + 1);
            }
        }
        
        game.showGameOverStats();
    }
};

#endif