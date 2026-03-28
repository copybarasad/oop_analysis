#include "include/MovementManager.h"
#include "include/Field.h"
#include "../../entities/include/Entity.h"
#include "../../entities/include/Enemy.h"
#include "../../entities/include/Player.h"

Position MovementManager::stepFrom(const Position& p, Direction d) {
    switch (d) {
        case Direction::Up: return Position{p.row - 1, p.col};
        case Direction::Down: return Position{p.row + 1, p.col};
        case Direction::Left: return Position{p.row, p.col - 1};
        case Direction::Right: return Position{p.row, p.col + 1};
        default: return p;
    }
}

bool MovementManager::tryMove(EntityId id, Direction dir) {
    auto* mover = em_.entity(id);
    if (!mover) return false;

    if (mover->type() == EntityType::Player) {
        auto* p = static_cast<Player*>(mover);
        if (p->isSlowed()) {
            p->setSlowed(false);
            return false;
        }
    }

    auto posOpt = em_.positionOf(id);
    if (!posOpt) return false;
    Position from = *posOpt;
    Position to = stepFrom(from, dir);

    if (!field_.inBounds(to) || !field_.cellAt(to).isPassable()) return false;

    if (auto occ = em_.occupantAt(to)) {
        EntityId targetId = *occ;
        Entity* dst = em_.entity(targetId);
        if (!dst) return false;

        if (mover->type() == EntityType::Enemy && dst->type() == EntityType::Player) {
            auto* enemy = static_cast<Enemy*>(mover);
            combat_.damageAndResolve(id, targetId, enemy->damage());
            return false;
        }
        if (mover->type() == EntityType::Player && (dst->type() == EntityType::Enemy || dst->type() == EntityType::EnemyBuilding || dst->type() == EntityType::EnemyTower)) {
            auto* player = static_cast<Player*>(mover);
            combat_.damageAndResolve(id, targetId, player->damage());
            return false;
        }
        return false;
    }

    if (!em_.moveTo(id, to)) return false;

    // Ловушка срабатывает только на врага при заходе в клетку.
    if (mover->type() == EntityType::Enemy) {
        int trapDmg = field_.cellAt(to).triggerTrap();
        if (trapDmg > 0) {
            combat_.damageAndResolve(InvalidEntityId, id, trapDmg);
            if (em_.entity(id) == nullptr) return false;
        }
    }

    // Замедлялка: игрок ход пропускает в следующий раз.
    if (mover->type() == EntityType::Player) {
        auto* p = static_cast<Player*>(mover);
        if (field_.cellAt(to).isSlowing()) p->setSlowed(true);
    }
    return true;
}
