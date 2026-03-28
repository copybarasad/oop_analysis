#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>

class Game;

class GameState {
private:
    Game& game;

public:
    explicit GameState(Game& game);

    int getCurrentLevel() const;
    void advanceLevel();

    int getPlayerScore() const;
    int getEnhancementLevel() const;
    void increaseEnhancementLevel();

    int getTurnCounter() const;
    void incrementTurnCounter();

    bool isPlayerSkippingTurn() const;
    void setPlayerSkippingTurn(bool value);

private:
    void validateState() const;
};

#endif