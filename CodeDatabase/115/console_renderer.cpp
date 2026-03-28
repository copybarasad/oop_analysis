#include "console_renderer.h"
#include "game_state.h"
#include "game_io.h" 

void ConsoleRenderer::render(const GameState& game_state) {
    GameIO::displayGameInfo(
        game_state.getPlayer(), 
        game_state.getEnemies(), 
        game_state.getEntityManager(), 
        game_state.getTowers()
    );
    GameIO::displayField(
        game_state.getField(), 
        game_state.getPlayer(), 
        game_state.getEnemies(), 
        game_state.getEntityManager(), 
        game_state.getTowers()
    );
}