#include "ConsoleRenderer.h"
#include "Game.h"
#include <iostream>

void ConsoleRenderer::render(const Game& game) {
    std::cout << "\n=== GAME STATE ===\n";
    displayStatus(game);
    std::cout << "\n";
    displayGameField(game);
    showControls(); 
}

void ConsoleRenderer::displayGameField(const Game& game) {
    game.displayGameField();
}

void ConsoleRenderer::displayStatus(const Game& game) {
    game.displayStatus();
}

void ConsoleRenderer::showControls() {
    std::cout << "\n=== CONTROLS ===\n";
    std::cout << "W - Move Up    A - Move Left\n";
    std::cout << "S - Move Down  D - Move Right\n";
    std::cout << "C - Switch Combat Mode (Melee/Ranged)\n";
    std::cout << "Q - Quit Game\n";
    std::cout << "LEGEND: P=Player, E=Enemy, #=Wall\n";
}

void ConsoleRenderer::showMessage(const std::string& message) {
    std::cout << message << std::endl;
}