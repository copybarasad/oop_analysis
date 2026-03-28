//
// Created by Artem on 02.10.2025.
//

#ifndef LABA1_CREATUREMANAGER_H
#define LABA1_CREATUREMANAGER_H
#include "../../../Entities/Creatures/Creature.h"
#include "../../../GameField/Field.h"
#include "../EntityManager.h"

class CreatureManager : public EntityManager{
public:
    CreatureManager(Field &field, Creature *creature) : EntityManager(field, creature), creature(creature) {}

    virtual void moveTo(Constants::dxdy withOffset) = 0;
    virtual int getStepRange() const noexcept final;
    virtual bool isCreatureDisabled() const noexcept final;
    virtual void enableCreature() noexcept final;
    virtual EntityType getEntityType() noexcept final;

protected:

private:
    Creature* creature;
};


#endif //LABA1_CREATUREMANAGER_H
