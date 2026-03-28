//
// Created by Artem on 02.10.2025.
//

#ifndef LABA1_ENEMYMANAGER_H
#define LABA1_ENEMYMANAGER_H
#include "../../../Entities/Creatures/CompControlledCreature.h"
#include "CreatureManager.h"
#include "../../../Entities/Creatures/Player.h"
#include "../../../Constants/Constants.h"
#include <iostream>

class CompControlledCreatureManager : public CreatureManager{
public:
    CompControlledCreatureManager(Field &field, std::shared_ptr<CompControlledCreature>& compControlledCreature) : CreatureManager(field, compControlledCreature.get()),
    compControlledCreature(compControlledCreature) {}

    void moveTo(Constants::dxdy dxdy) override;

    virtual void attack(Constants::XYPair coordinates);

    virtual int getChanceToDetectHostile() noexcept final;

protected:
    std::shared_ptr<CompControlledCreature> compControlledCreature;
};


#endif //LABA1_ENEMYMANAGER_H
