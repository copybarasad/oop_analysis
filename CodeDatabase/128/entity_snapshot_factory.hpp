#pragma once
#include "entity_snapshot.hpp"
#include "gamemodel.hpp"
#include "enemy.hpp"
#include "ally.hpp"
#include "trap.hpp"
#include "enemy_building.hpp"
#include "enemytower.hpp"

class EntitySnapshotFactory {
public:
    EntitySnapshotFactory() = default;

    EntitySnapshot MakePlayerSnapshot(const Game& game) const;
    EntitySnapshot MakeEnemySnapshot(const EnemyUnit& e) const;
    EntitySnapshot MakeAllySnapshot(const Ally& ally) const;
    EntitySnapshot MakeTrapSnapshot(const Trap& trap) const;
    EntitySnapshot MakeTowerSnapshot(const EnemyTower& tower) const;
    EntitySnapshot MakeBuildingSnapshot(const EnemyBuilding& building) const;
};
