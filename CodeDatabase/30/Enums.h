#pragma once
#include <cstddef>

enum class EntityType {
    Player,
    Enemy,
    EnemyBuilding,
    EnemyTower,
    Ally
};

enum class AttackMode {
    Melee,
    Ranged
};

enum class Direction {
    None,
    Up,
    Down,
    Left,
    Right
};

using EntityId = std::size_t;
static constexpr EntityId InvalidEntityId = static_cast<EntityId>(-1);
