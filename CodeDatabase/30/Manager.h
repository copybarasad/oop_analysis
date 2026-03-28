#pragma once
#include <vector>
#include <optional>
#include <memory>
#include <random>
#include <unordered_map>
#include "Enums.h"
#include "Field.h"
#include "Position.h"
#include "../../entities/include/EntityRegistry.h"
#include "../../entities/include/Player.h"
#include "../../entities/include/Enemy.h"
#include "../../entities/include/EnemyBuilding.h"
#include "EntityManager.h"
#include "CombatManager.h"
#include "TurnsManager.h"
#include "MovementManager.h"

class Manager {
public:
    explicit Manager(Field field);

    const Field& field() const;
    Field& field();

    int rows() const;
    int cols() const;
    bool inBounds(const Position& p) const;
    bool isPassable(const Position& p) const;
    bool isSlowing(const Position& p) const;

    std::optional<EntityId> occupantAt(const Position& p) const;
    bool canEnter(const Position& p) const;

    void enemyTurn();
    void buildingsTurn();
    void alliesTurn();

    Entity* entity(EntityId id);
    const Entity* entity(EntityId id) const;
    Entity* entityAt(const Position& p);
    const Entity* entityAt(const Position& p) const;

    void save(std::ostream& os) const;
    void load(std::istream& is);

private:
    Field field_;
    EntityManager entityMgr_;
    CombatManager combatMgr_;
    MovementManager movementMgr_;
    TurnsManager turnsMgr_;
    
public:
    EntityManager& entities() { return entityMgr_; }
    const EntityManager& entities() const { return entityMgr_; }
    CombatManager& combat() { return combatMgr_; }
    const CombatManager& combat() const { return combatMgr_; }
    TurnsManager& turns() { return turnsMgr_; }
    const TurnsManager& turns() const { return turnsMgr_; }
    MovementManager& movement() { return movementMgr_; }
    const MovementManager& movement() const { return movementMgr_; }
};
