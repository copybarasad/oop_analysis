//
// Created by bob on 10/15/25.
//

#ifndef GAME_TERM_ENEMYSPAWNER_H
#define GAME_TERM_ENEMYSPAWNER_H
#include "Field.h"


class EnemySpawner {

    std::shared_ptr<Field> field;

public:

    explicit EnemySpawner(std::shared_ptr<Field> field);

    void spawnEnemies();
};


#endif //GAME_TERM_ENEMYSPAWNER_H