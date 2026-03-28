//
// Created by Artem on 30.10.2025.
//

#ifndef LABAOOP2_ENEMYDEFENCETOWERMANAGER_H
#define LABAOOP2_ENEMYDEFENCETOWERMANAGER_H

#include "../EntityManager.h"
#include "../../../Entities/Buildings/EnemyDefenceTower.h"
#include "../../../Constants/Constants.h"

class EnemyDefenceTowerManager : public EntityManager{
public:
    EnemyDefenceTowerManager(Field &field, int stepCounter, std::shared_ptr<EnemyDefenceTower> &tower) : EntityManager(field, tower.get()),
                                                                                                         counter(stepCounter), tower(tower) {}
    EnemyDefenceTowerManager(Field &field, std::shared_ptr<EnemyDefenceTower> &tower) : EntityManager(field, tower.get()),
                                                                                                         counter(0), tower(tower) {}
    virtual bool isCoolDowned() const noexcept final;
    virtual void goToCoolDown() noexcept final;
    virtual void doCoolDownStep() noexcept final;
    virtual int getAttackRadius() const noexcept final;
    virtual void attackCoordinates(Constants::XYPair coords) final; //сюда передаются выбранные координаты из контроллера, они точно правильные -> проверки не требуются + есть проверки в методе кастаЁ!
private:
    int counter;
    std::shared_ptr<EnemyDefenceTower> tower;
};


#endif //LABAOOP2_ENEMYDEFENCETOWERMANAGER_H
