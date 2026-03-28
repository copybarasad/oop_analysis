//
// Created by Artem on 06.10.2025.
//

#ifndef LABA1_ENTITYMANAGER_H
#define LABA1_ENTITYMANAGER_H


#include "../../Entities/Creatures/Creature.h"
#include "../../GameField/Field.h"
#include "../../Constants/Constants.h"

class EntityManager {
public:
    virtual bool isAlive() const noexcept final{
        return this->entity->isAlive();
    }
    virtual Constants::XYPair getEntityCoords() const noexcept final{
        return Constants::XYPair{this->entity->getXCoordinate(), this->entity->getYCoordinate()};
    }

    EntityManager(Field &field, Entity *entity) : field(field), entity(entity) {}

protected:
    Field &field;
private:
    Entity* entity;
};

#endif //LABA1_ENTITYMANAGER_H
