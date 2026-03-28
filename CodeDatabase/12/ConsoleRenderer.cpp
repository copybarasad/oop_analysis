#include "ConsoleRenderer.h"
#include "Game.h"

void ConsoleRenderer::render(const Game& game) const {
    game.render();
}
