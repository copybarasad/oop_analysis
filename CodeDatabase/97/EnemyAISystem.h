#ifndef UNTITLED_ENEMYAISYSTEM_H
#define UNTITLED_ENEMYAISYSTEM_H

#include "Coord.h"
#include <vector>

class GameModel;
class MovementSystem;

class EnemyAISystem {
public:
    void step(GameModel& model, MovementSystem& mover);
};

#endif // UNTITLED_ENEMYAISYSTEM_H
