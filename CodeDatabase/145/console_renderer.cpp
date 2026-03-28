#include "console_renderer.h"
#include "game.h"
#include "game_state_manager.h"
#include "game_initializer.h"
#include <iostream>

void ConsoleRenderer::renderField(const Game& game) {
    const_cast<Game&>(game).stateManager->displayGameField();
}

void ConsoleRenderer::renderPlayerStatus(const Game& game) {
    const_cast<Game&>(game).stateManager->displayPlayerStatus();
}

void ConsoleRenderer::renderSpellList(const Game& game) {
    const_cast<Game&>(game).stateManager->displaySpellList();
}

void ConsoleRenderer::renderMessage(const std::string& message) {
    std::cout << message << "\n";
}

void ConsoleRenderer::renderControls() {
    GameInitializer::displayControls();
}