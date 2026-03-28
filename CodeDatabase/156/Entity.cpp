//
// Created by Artem on 24.09.2025.
//

#include "Entity.h"

int Entity::getXCoordinate() const noexcept{
    return xCoordinate;
}

int Entity::getYCoordinate() const noexcept{
    return yCoordinate;
}

EntityType Entity::getType() const noexcept{
    return type;
}

int Entity::getHealthPoints() const noexcept {
    return healthPoints;
}

void Entity::changeHealthPoints(int dHP) noexcept {
    this->healthPoints = this->healthPoints + dHP;
}

bool Entity::isAlive() const noexcept {
    return this->healthPoints > 0;
}