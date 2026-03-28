#include "entity_snapshot_factory.hpp"

EntitySnapshot EntitySnapshotFactory::MakePlayerSnapshot(const Game& game) const {
    const auto& p = game.getPlayer();
    return EntitySnapshot(
        -1,
        EntityType::Player,
        p.getHealth(),
        p.getDamage(),
        { p.getX(), p.getY() }
    );
}

EntitySnapshot EntitySnapshotFactory::MakeEnemySnapshot(const EnemyUnit& e) const {
    return EntitySnapshot(
        e.id,
        EntityType::Enemy,
        e.enemy.getHealth(),
        e.enemy.getDamage(),
        { e.x, e.y }
    );
}

EntitySnapshot EntitySnapshotFactory::MakeAllySnapshot(const Ally& ally) const {
    return EntitySnapshot(
        -1,
        EntityType::Ally,
        ally.getHealth(),
        ally.getDamage(),
        { ally.getX(), ally.getY() }
    );
}

EntitySnapshot EntitySnapshotFactory::MakeTrapSnapshot(const Trap& trap) const {
    return EntitySnapshot(
        -1,
        EntityType::Trap,
        trap.getDamage(),  
        0,
        { trap.getX(), trap.getY() }
    );
}

EntitySnapshot EntitySnapshotFactory::MakeTowerSnapshot(const EnemyTower& t) const {
    return EntitySnapshot(
        -1,
        EntityType::EnemyTower,
        t.getHealth(),
        t.getDamage(),
        { t.getX(), t.getY() }
    );
}

EntitySnapshot EntitySnapshotFactory::MakeBuildingSnapshot(const EnemyBuilding& b) const {
    return EntitySnapshot(
        -1,
        EntityType::EnemyBuilding,
        b.getHealth(),
        b.getDamage(),
        { b.getX(), b.getY() }
    );
}
