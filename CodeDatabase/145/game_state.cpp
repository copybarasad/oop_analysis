#include "game_state.h"
#include "game.h"
#include <stdexcept>

GameState::GameState(Game& game) : game(game) {
    validateState();
}

int GameState::getCurrentLevel() const {
    return game.currentLevel;
}

void GameState::advanceLevel() {
    game.currentLevel++;
}

int GameState::getPlayerScore() const {
    return game.getPlayerScore();
}

int GameState::getEnhancementLevel() const {
    return game.enhancementLevel;
}

void GameState::increaseEnhancementLevel() {
    game.enhancementLevel++;
}

int GameState::getTurnCounter() const {
    return game.playerTurnCounter;
}

void GameState::incrementTurnCounter() {
    game.playerTurnCounter++;
}

bool GameState::isPlayerSkippingTurn() const {
    return game.playerSkippingTurn;
}

void GameState::setPlayerSkippingTurn(bool value) {
    game.playerSkippingTurn = value;
}

void GameState::validateState() const {
    if (game.currentLevel < 1) {
        throw std::runtime_error("Current level must be >= 1");
    }
    if (game.enhancementLevel < 0) {
        throw std::runtime_error("Enhancement level cannot be negative");
    }
}