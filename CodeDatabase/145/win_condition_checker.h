#ifndef WIN_CONDITION_CHECKER_H
#define WIN_CONDITION_CHECKER_H

class Game;

class WinConditionChecker {
public:
    static bool isGameOver(const Game& game);
    static bool playerWon(const Game& game);
    static bool playerLost(const Game& game);
};

#endif