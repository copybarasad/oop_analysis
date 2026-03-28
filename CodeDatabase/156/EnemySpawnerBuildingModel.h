//
// Created by Artem on 17.11.2025.
//

#ifndef LABAOOP2_ENEMYSPAWNERBUILDINGMODEL_H
#define LABAOOP2_ENEMYSPAWNERBUILDINGMODEL_H


class EnemySpawnerBuildingModel {
public:
    EnemySpawnerBuildingModel(int healthPoint, int spawnInterval) : healthPoint(healthPoint),
                                                                    spawnInterval(spawnInterval) {}

    int healthPoint, spawnInterval;
};


#endif //LABAOOP2_ENEMYSPAWNERBUILDINGMODEL_H
