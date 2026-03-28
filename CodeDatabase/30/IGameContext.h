#pragma once

#include "Enums.h"
#include "Position.h"
#include <memory>
#include <optional>

class Entity;

class IGameContext {
public:
    virtual ~IGameContext() = default;

    virtual bool inBounds(const Position& p) const = 0;
    virtual bool isPassable(const Position& p) const = 0;
    virtual bool canEnter(const Position& p) const = 0;

    virtual std::optional<EntityId> occupantAt(const Position& p) const = 0;
    virtual std::optional<Position> positionOf(EntityId id) const = 0;
    virtual Entity* entity(EntityId id) = 0;
    virtual const Entity* entity(EntityId id) const = 0;
    virtual Entity* entityAt(const Position& p) = 0;
    virtual const Entity* entityAt(const Position& p) const = 0;

    virtual EntityId spawnAt(std::unique_ptr<Entity> e, const Position& p) = 0;
    virtual bool remove(EntityId id) = 0;
};
