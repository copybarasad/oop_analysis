//
// Created by Artem on 05.10.2025.
//

#ifndef LABA1_ENEMYMODEL_H
#define LABA1_ENEMYMODEL_H


class CompControlledCreatureModel {
public:
    int healthPoint, damage, stepCount, chanceToDetectHostile;

    CompControlledCreatureModel(int healthPoint, int damage, int stepCount, int chanceToDetectHostile) : healthPoint(healthPoint), damage(damage),
                                                                                                         stepCount(stepCount), chanceToDetectHostile(chanceToDetectHostile) {}

};


#endif //LABA1_ENEMYMODEL_H
