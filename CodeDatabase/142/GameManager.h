#pragma once

#include "Command.h"
#include "Game.h"

template <typename InputProvider>
class GameManager {
public:
    explicit GameManager(Game& gameRef) : game(gameRef) {}

    void processPlayerTurn() {
        InputProvider provider;
        Command cmd = provider.readCommand(game);
        bool turnUsed = game.applyCommand(cmd);
        game.handlePostPlayerAction(turnUsed);
    }

private:
    Game& game;
};

