#pragma once
#include <unordered_map>
#include <vector>
#include <optional>
#include "Enums.h"
#include "Position.h"
#include "Field.h"
#include "IGameContext.h"
#include "../../entities/include/EntityRegistry.h"

class EntityManager : public IGameContext {
public:
    EntityManager(const Field& field);

    bool inBounds(const Position& p) const override { return field_.inBounds(p); }
    bool isPassable(const Position& p) const override { return field_.cellAt(p).isPassable(); }
    bool canEnter(const Position& p) const override {
        if (!inBounds(p)) return false;
        if (!isPassable(p)) return false;
        return !occupant_[p.row][p.col].has_value();
    }
    std::optional<EntityId> occupantAt(const Position& p) const override {
        if (!inBounds(p)) return std::nullopt;
        return occupant_[p.row][p.col];
    }
    std::optional<Position> positionOf(EntityId id) const override {
        auto it = posById_.find(id); if (it==posById_.end()) return std::nullopt; return it->second; }
    Entity* entity(EntityId id) override { return registry_.get(id); }
    const Entity* entity(EntityId id) const override { return registry_.get(id); }
    Entity* entityAt(const Position& p) override { auto occ=occupantAt(p); return occ?registry_.get(*occ):nullptr; }
    const Entity* entityAt(const Position& p) const override { auto occ=occupantAt(p); return occ?registry_.get(*occ):nullptr; }
    EntityId spawnAt(std::unique_ptr<Entity> e, const Position& p) override;
    bool remove(EntityId id) override;

    bool moveTo(EntityId id, const Position& to);
    bool switchPlayerMode(EntityId playerId, AttackMode mode);
    bool spawnEnemyNear(EntityId buildingId);
    std::vector<EntityId> listByType(EntityType type) const;
    EntityId findPlayerId() const;

    void save(std::ostream& os) const;
    void load(std::istream& is);

    const EntityRegistry& registry() const { return registry_; }

private:
    const Field& field_;
    EntityRegistry registry_;
    std::unordered_map<EntityId, Position> posById_;
    std::vector<std::vector<std::optional<EntityId>>> occupant_;
};
