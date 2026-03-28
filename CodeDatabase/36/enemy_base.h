#ifndef ENEMY_BASE_H
#define ENEMY_BASE_H

#include "character.h"
#include <utility>

class EnemyBase : public Character {
public:
    EnemyBase(int row, int col, int spawn_interval, int health = 15);
    std::pair<int,int> Position() const;
    bool ShouldSpawn();

    int GetSpawnInterval() const { return spawn_interval_; }
    int GetCounter() const { return counter_; }
    void SetCounter(int counter);

private:
    int row_, col_;
    int spawn_interval_;
    int counter_;
};

#endif
