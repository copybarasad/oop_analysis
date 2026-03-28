#ifndef GAME_END_HANDLER_H
#define GAME_END_HANDLER_H

class Game;

class GameEndHandler {
public:
    static bool checkPlayerWon(Game& game);
    static bool handleGameEnd(Game& game);
    static void displayFinalScore(const Game& game);
};

#endif