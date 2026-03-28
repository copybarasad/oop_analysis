//
// Created by Artem on 08.10.2025.
//

#ifndef LABA1_ENEMYCONTROLLER_H
#define LABA1_ENEMYCONTROLLER_H
#include "CompControlledCreatureController.h"

class EnemyController : public CompControlledCreatureController{
public:
    EnemyController(const Field &field, const CompControlledCreatureManager &manager): CompControlledCreatureController(field, manager) {}

    void doMove(GameMasterBase& gameMaster);


private:
    std::map<EntityType, bool> typesToAttack = {{EntityType::PlayerEnt, true}, {EntityType::Ally, true}};
    std::vector<EntityType> priorityOfAttack = {EntityType::PlayerEnt, EntityType::Ally};
};


#endif //LABA1_ENEMYCONTROLLER_H
