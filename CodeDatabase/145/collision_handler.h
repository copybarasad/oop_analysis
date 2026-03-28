#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <utility>

class Enemy;
class Ally;
class Player;
class GameField;
class Game;

class CollisionHandler {
public:
    static void handleEnemyPlayerCollision(Enemy& enemy, Player& player, Game& game);
    static void handleEnemyPlayerCollision(Enemy& enemy, Player& player);

    static void handleAllyEnemyCollision(Ally& ally, Enemy& enemy, Player& player);

    static void handleTrapCollision(const std::pair<int, int>& position, Game& game);

private:
    static const int COLLISION_DAMAGE = 5;
};

#endif