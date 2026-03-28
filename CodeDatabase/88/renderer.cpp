#include "renderer.hpp"
#include "../managers/gamemanager.hpp"

void ConsoleRenderer::render(const Game& game) const {   
    game.displayGame();
}