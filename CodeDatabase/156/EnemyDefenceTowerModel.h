//
// Created by Artem on 17.11.2025.
//

#ifndef LABAOOP2_ENEMYDEFENCETOWERMODEL_H
#define LABAOOP2_ENEMYDEFENCETOWERMODEL_H

class EnemyDefenceTowerModel {
public:
    EnemyDefenceTowerModel(int healthPoint, int attackRadius, int attackInterval) : healthPoint(healthPoint),
                                                                                    attackRadius(attackRadius),
                                                                                    attackInterval(attackInterval) {}

    int healthPoint, attackRadius, attackInterval;

};


#endif //LABAOOP2_ENEMYDEFENCETOWERMODEL_H
