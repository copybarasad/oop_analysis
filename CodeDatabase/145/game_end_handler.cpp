#include "game_end_handler.h"
#include "game.h"
#include "game_input_handler.h"
#include "level_manager.h"
#include <iostream>

bool GameEndHandler::checkPlayerWon(Game& game) {
    return game.enemies.empty() && game.player.isAlive();
}

bool GameEndHandler::handleGameEnd(Game& game) {
    displayFinalScore(game);
    
    if (!checkPlayerWon(game)) {
        return GameInputHandler::getYesNoChoice("You lost! Start again? (y/n): ");
    }
    
    return false;
}

void GameEndHandler::displayFinalScore(const Game& game) {
    std::cout << "Final score: " << game.getPlayerScore() << " points\n";
}