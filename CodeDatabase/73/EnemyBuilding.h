#ifndef GAMELB1_ENEMYBUILDING_H
#define GAMELB1_ENEMYBUILDING_H

#include "Entity.h"

class EnemyBuilding : public Entity {
public:
    EnemyBuilding(int x, int y, int spawn_interval);

    bool update();


private:
    int spawn_interval_;
    int spawn_timer_;
};

#endif //GAMELB1_ENEMYBUILDING_H