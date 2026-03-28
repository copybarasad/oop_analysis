#include "../include/Manager.h"
#include <cassert>
#include "../../entities/include/EnemyTower.h"
#include "../../entities/include/Ally.h"
#include "../include/GameException.h"

Manager::Manager(Field field)
        : field_(std::move(field)),
            entityMgr_(field_),
            combatMgr_(entityMgr_),
            movementMgr_(entityMgr_, combatMgr_, field_),
            turnsMgr_(*this, movementMgr_, entityMgr_) {}

const Field& Manager::field() const { return field_; }

Field& Manager::field() { return field_; }

int Manager::rows() const { return field_.rows(); }

int Manager::cols() const { return field_.cols(); }

bool Manager::inBounds(const Position& p) const { return field_.inBounds(p); }

bool Manager::isPassable(const Position& p) const { return field_.cellAt(p).isPassable(); }

bool Manager::isSlowing(const Position& p) const { return field_.cellAt(p).isSlowing(); }

std::optional<EntityId> Manager::occupantAt(const Position& p) const { return entityMgr_.occupantAt(p); }

bool Manager::canEnter(const Position& p) const { return entityMgr_.canEnter(p); }

Entity* Manager::entity(EntityId id) { return entityMgr_.entity(id); }
const Entity* Manager::entity(EntityId id) const { return entityMgr_.entity(id); }
Entity* Manager::entityAt(const Position& p) { return entityMgr_.entityAt(p); }
const Entity* Manager::entityAt(const Position& p) const { return entityMgr_.entityAt(p); }

void Manager::enemyTurn() {
    turnsMgr_.enemyTurn();
}

void Manager::buildingsTurn() {
    turnsMgr_.buildingsTurn();
}

void Manager::alliesTurn() {
    turnsMgr_.alliesTurn();
}

void Manager::save(std::ostream& os) const {
    field_.save(os);
    entityMgr_.save(os);
}

void Manager::load(std::istream& is) {
    field_.load(is);
    entityMgr_.load(is);
}
