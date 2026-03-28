//
// Created by bob on 10/4/25.
//

#include "Entity.h"


Entity::Entity(const Position &position): position(position) {}

void Entity::setPosition(const Position &position) { this->position = position; }

Position Entity::getPosition() const { return this->position; }

void Entity::move(const Position &delta) {
    this->position += delta;
}
