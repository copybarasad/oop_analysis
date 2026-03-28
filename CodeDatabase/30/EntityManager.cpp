#include "include/EntityManager.h"
#include "../../entities/include/Player.h"
#include "../../entities/include/EnemyBuilding.h"
#include "../../entities/include/Enemy.h"
#include "../../entities/include/EnemyTower.h"
#include "../../entities/include/Ally.h"
#include "include/GameException.h"
#include <random>

EntityManager::EntityManager(const Field& field)
    : field_(field),
      occupant_(field.rows(), std::vector<std::optional<EntityId>>(field.cols(), std::nullopt))
{}

EntityId EntityManager::spawnAt(std::unique_ptr<Entity> e, const Position& p) {
    if (!canEnter(p)) return InvalidEntityId;
    EntityId id = registry_.create(std::move(e));
    if (id == InvalidEntityId) return InvalidEntityId;
    posById_[id] = p;
    occupant_[p.row][p.col] = id;
    return id;
}

bool EntityManager::remove(EntityId id) {
    if (!registry_.alive(id)) return false;
    auto it = posById_.find(id);
    if (it == posById_.end()) return false;
    Position p = it->second;
    if (inBounds(p) && occupant_[p.row][p.col] == id) {
        occupant_[p.row][p.col].reset();
    }
    posById_.erase(id);
    registry_.destroy(id);
    return true;
}

bool EntityManager::moveTo(EntityId id, const Position& to) {
    auto it = posById_.find(id);
    if (it == posById_.end()) return false;
    Position from = it->second;
    if (!inBounds(to)) return false;
    if (occupant_[to.row][to.col].has_value()) return false;
    occupant_[from.row][from.col].reset();
    occupant_[to.row][to.col] = id;
    it->second = to;
    return true;
}

bool EntityManager::switchPlayerMode(EntityId playerId, AttackMode mode) {
    Entity* ent = registry_.get(playerId);
    if (!ent || ent->type() != EntityType::Player) return false;
    Player* p = static_cast<Player*>(ent);
    if (mode == AttackMode::Melee) p->switchToMelee(); else p->switchToRanged();
    return true;
}

bool EntityManager::spawnEnemyNear(EntityId buildingId) {
    Entity* ent = registry_.get(buildingId);
    if (!ent || ent->type() != EntityType::EnemyBuilding) return false;
    auto posOpt = positionOf(buildingId);
    if (!posOpt) return false;
    Position center = *posOpt;

    std::vector<Position> candidates;
    for (int dr = -2; dr <= 2; ++dr) {
        for (int dc = -2; dc <= 2; ++dc) {
            if (dr == 0 && dc == 0) continue;
            Position np{center.row + dr, center.col + dc};
            if (!inBounds(np)) continue;
            if (!field_.cellAt(np).isPassable()) continue;
            if (occupant_[np.row][np.col].has_value()) continue;
            candidates.push_back(np);
        }
    }
    if (candidates.empty()) return false;

    std::random_device rd; std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(candidates.size()) - 1);
    Position chosen = candidates[dist(gen)];

    auto* building = static_cast<EnemyBuilding*>(ent);
    auto enemy = building->spawnEnemy();
    return spawnAt(std::move(enemy), chosen) != InvalidEntityId;
}

std::vector<EntityId> EntityManager::listByType(EntityType type) const {
    std::vector<EntityId> out;
    out.reserve(posById_.size());
    for (auto& kv : posById_) {
        EntityId id = kv.first;
        const Entity* e = registry_.get(id);
        if (e && e->type() == type) out.push_back(id);
    }
    return out;
}

EntityId EntityManager::findPlayerId() const {
    for (auto& kv : posById_) {
        EntityId id = kv.first;
        const Entity* e = registry_.get(id);
        if (e && e->type() == EntityType::Player) return id;
    }
    return InvalidEntityId;
}

void EntityManager::save(std::ostream& os) const {
    registry_.saveState(os);
    
    os << posById_.size() << "\n";
    for (const auto& kv : posById_) {
        EntityId id = kv.first;
        Position pos = kv.second;
        const Entity* e = registry_.get(id);
        if (!e) continue;
        
        os << id << " " << pos.row << " " << pos.col << "\n";
        e->save(os);
    }
}

void EntityManager::load(std::istream& is) {
    posById_.clear();
    for (auto& row : occupant_) {
        std::fill(row.begin(), row.end(), std::nullopt);
    }
    
    occupant_.assign(field_.rows(), std::vector<std::optional<EntityId>>(field_.cols(), std::nullopt));
    
    registry_.loadState(is);
    
    size_t count;
    if (!(is >> count)) throw GameSaveException("Failed to read entity count");
    
    for (size_t i = 0; i < count; ++i) {
        EntityId id;
        int r, c;
        if (!(is >> id >> r >> c)) throw GameSaveException("Failed to read entity header");
        Position pos{r, c};

        if (!field_.inBounds(pos)) {
            throw GameSaveException("Entity position out of bounds: " + std::to_string(r) + "," + std::to_string(c));
        }
        
        int typeInt;
        if (!(is >> typeInt)) throw GameSaveException("Failed to read entity type");
        EntityType type = static_cast<EntityType>(typeInt);
        
        std::unique_ptr<Entity> e;
        switch (type) {
            case EntityType::Player: e = std::make_unique<Player>(1, 1); break;
            case EntityType::Enemy: e = std::make_unique<Enemy>(1, 1); break;
            case EntityType::EnemyBuilding: e = std::make_unique<EnemyBuilding>(1, 1, 1, 1); break;
            case EntityType::EnemyTower: e = std::make_unique<EnemyTower>(1, 1, 1, 1); break;
            case EntityType::Ally: e = std::make_unique<Ally>(1, 1); break;
            default: throw GameSaveException("Unknown entity type");
        }
        
        e->load(is);
        
        registry_.restore(id, std::move(e));
        posById_[id] = pos;
        if (field_.inBounds(pos)) {
            occupant_[pos.row][pos.col] = id;
        }
    }
}
