#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "renderer_interface.h"
#include "game.h"
#include "app_state.h"

template <typename TRenderer>
class Visualizer {
public:
    Visualizer(TRenderer& renderer) 
        : renderer(renderer) {}
    
    void updateAndRender(Game& game, 
                        AppState state, 
                        int cursorX = 0, 
                        int cursorY = 0, 
                        int spellIndex = 0) {
        renderer.render(&game, state, cursorX, cursorY, spellIndex);
    }
    
    void onGameStateChanged(Game& game, 
                           AppState newState, 
                           int cursorX = 0, 
                           int cursorY = 0, 
                           int spellIndex = 0) {
        updateAndRender(game, newState, cursorX, cursorY, spellIndex);
    }
    
    void onPlayerAction(Game& game, 
                       const std::string& action, 
                       int cursorX = 0, 
                       int cursorY = 0, 
                       int spellIndex = 0) {
        updateAndRender(game, AppState::GAME, cursorX, cursorY, spellIndex);
    }
    
    void onEnemyTurn(Game& game) {
        updateAndRender(game, AppState::GAME);
    }

private:
    TRenderer& renderer;
};

#endif