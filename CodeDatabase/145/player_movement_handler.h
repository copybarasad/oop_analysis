#ifndef PLAYER_MOVEMENT_HANDLER_H
#define PLAYER_MOVEMENT_HANDLER_H

#include <utility>

class Game;
class Player;
class GameField;

class PlayerMovementHandler {
public:
    static void moveUp(Game& game);
    static void moveDown(Game& game);
    static void moveLeft(Game& game);
    static void moveRight(Game& game);

private:
    static void handleMovement(Game& game, int dx, int dy);
    static bool canMoveToPosition(const Game& game, int x, int y);
    static void executeMovement(Game& game, const std::pair<int, int>& newPos);
};

#endif
