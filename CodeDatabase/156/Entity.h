//
// Created by Artem on 24.09.2025.
//

#ifndef LABA1_ENTITY_H
#define LABA1_ENTITY_H
#include "EntityType.h"
#include <memory>

class Entity {
private:

public:
    Entity(int xCoordinate, int yCoordinate, int healthPoint, EntityType type) : xCoordinate(xCoordinate),
                                                                                 yCoordinate(yCoordinate),
                                                                                 healthPoints(healthPoint),
                                                                                 type(type) {
    }

    Entity(const Entity& other) = default;
    Entity(Entity&& other) noexcept = default;

    virtual int getXCoordinate() const noexcept final;

    virtual int getYCoordinate() const noexcept final;

    virtual EntityType getType() const noexcept final;

    virtual int getHealthPoints() const noexcept final;

    virtual void changeHealthPoints(int dHP) noexcept final;

    virtual bool isAlive() const noexcept final;

    virtual std::unique_ptr<Entity> clone() const = 0;
    virtual std::string serialize() = 0;

    virtual ~Entity() = default;

protected:
    int xCoordinate, yCoordinate, healthPoints;
    EntityType type;
};


#endif //LABA1_ENTITY_H
