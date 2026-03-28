#include "include/TurnsManager.h"
#include "include/Manager.h"
#include "include/EntityManager.h"
#include "../../entities/include/EnemyBuilding.h"
#include "../../entities/include/EnemyTower.h"
#include "../../entities/include/Player.h"
#include "../../entities/include/Ally.h"

Direction TurnsManager::chooseStepTowards(const Position& from, const Position& to) const {
    int dr = to.row - from.row;
    int dc = to.col - from.col;
    if (std::abs(dr) >= std::abs(dc)) {
        if (dr > 0) return Direction::Down;
        if (dr < 0) return Direction::Up;
    } else {
        if (dc > 0) return Direction::Right;
        if (dc < 0) return Direction::Left;
    }
    return Direction::None;
}

void TurnsManager::enemyTurn() {
    EntityId playerId = em_.findPlayerId();
    if (playerId == InvalidEntityId) return;
    auto playerPosOpt = em_.positionOf(playerId);
    if (!playerPosOpt) return;
    Position playerPos = *playerPosOpt;

    auto enemies = em_.listByType(EntityType::Enemy);
    for (EntityId eid : enemies) {
        auto fromOpt = em_.positionOf(eid);
        if (!fromOpt) continue;
        Direction dir = chooseStepTowards(*fromOpt, playerPos);
        if (dir != Direction::None) {
            movement_.tryMove(eid, dir);
        }
    }
}

void TurnsManager::buildingsTurn() {
    // Спавн врагов зданиями
    auto buildings = em_.listByType(EntityType::EnemyBuilding);
    for (EntityId bid : buildings) {
        auto* b = static_cast<EnemyBuilding*>(em_.entity(bid));
        if (!b) continue;
        if (b->timeToSpawn()) {
            em_.spawnEnemyNear(bid);
        }
    }

    // Атака башен по игроку
    EntityId playerId = em_.findPlayerId();
    if (playerId == InvalidEntityId) return;
    auto playerPosOpt = em_.positionOf(playerId);
    if (!playerPosOpt) return;
    Position playerPos = *playerPosOpt;

    auto towers = em_.listByType(EntityType::EnemyTower);
    for (EntityId tid : towers) {
        auto* t = static_cast<EnemyTower*>(em_.entity(tid));
        if (!t) continue;
        t->tick();
        if (t->ready()) {
            if (!t->spell().hasCharges()) t->spell().addCharges(1);
            if (t->spell().canCast(manager_, tid, playerPos)) {
                t->spell().cast(manager_, tid, playerPos);
            }
            t->resetCooldown();
        }
    }
}

void TurnsManager::alliesTurn() {
    // Союзники приближаются к ближайшему врагу (Enemy / EnemyTower / EnemyBuilding) и атакуют если рядом
    auto allyIds = em_.listByType(EntityType::Ally);
    if (allyIds.empty()) return;

    // Собираем потенциальные цели
    std::vector<EntityId> targets;
    {
        auto enemies = em_.listByType(EntityType::Enemy); targets.insert(targets.end(), enemies.begin(), enemies.end());
        auto towers = em_.listByType(EntityType::EnemyTower); targets.insert(targets.end(), towers.begin(), towers.end());
        auto buildings = em_.listByType(EntityType::EnemyBuilding); targets.insert(targets.end(), buildings.begin(), buildings.end());
    }
    if (targets.empty()) return;

    for (EntityId aid : allyIds) {
        auto aposOpt = em_.positionOf(aid); if (!aposOpt) continue; Position apos = *aposOpt;
        // найти ближайшую цель по манхэттену
        int bestDist = 1e9; EntityId bestTarget = InvalidEntityId; Position bestPos{};
        for (EntityId tid : targets) {
            auto tposOpt = em_.positionOf(tid); if (!tposOpt) continue; Position tpos = *tposOpt;
            int dist = std::abs(tpos.row - apos.row) + std::abs(tpos.col - apos.col);
            if (dist < bestDist) { bestDist = dist; bestTarget = tid; bestPos = tpos; }
        }
        if (bestTarget == InvalidEntityId) continue;
        // Если рядом (dist == 1) — атакуем целевую клетку
        if (bestDist == 1) {
            auto* allyEnt = static_cast<Ally*>(em_.entity(aid));
            if (!allyEnt) continue;
            // Наносим урон цели
            manager_.combat().damageAndResolve(aid, bestTarget, allyEnt->damage());
            continue;
        }
        // Иначе двигаемся на один шаг в сторону цели
        Direction dir = chooseStepTowards(apos, bestPos);
        if (dir != Direction::None) {
            movement_.tryMove(aid, dir);
        }
    }
}
