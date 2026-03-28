#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include <vector>
#include "Board.h"
#include "Player.h"
#include "Enemy.h"

class EnemyBase{
public:
    EnemyBase(std::pair<int, int> pos, int period);
    std::pair<int, int> position();

    void onTurn(std::vector<Enemy>& enemies, Board& board, Player &player);

    int hp() const;
    bool isAlive() const;
    void takeDamage(int dmg);

    int spawnDelay() const;
    int currentTimer() const;

    void setHp(int hp);
    void setCurrentTimer(int value);

private:
    std::pair<int, int> pos_;
    int spawnDelay_;
    int currentDelay_;
    int hp_;
};

#endif