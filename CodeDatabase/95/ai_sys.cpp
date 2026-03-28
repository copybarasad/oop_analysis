#include "ai_sys.hpp"
#include "move_sys.hpp"
#include "fight_sys.hpp"
#include "game/entity_manager.hpp"
#include "game_objects/entity.hpp"
#include "game_objects/spells/damage_spell.hpp"

#include <iostream>

void AISystem::EnemyTurn(eManager& manager) {
    for (Entity* e : manager.getEnemies()) {
        if (e && e->isAlive()) {
            processEnemy(manager, *e);
        }
    }
}

void AISystem::processEnemy(eManager& manager, Entity& enemy) {
    Entity* player = manager.getPlayer();
    if (!player || !player->isAlive())
        return;

    Position ep = enemy.getPos();
    Position pp = player->getPos();

    int diffX = pp.x - ep.x;
    int diffY = pp.y - ep.y;

    int dx = 0;
    int dy = 0;

    if (std::abs(diffX) >= std::abs(diffY)) {
        dx = (diffX > 0) ? 1 : -1;
    } else {
        dy = (diffY > 0) ? 1 : -1;
    }

    auto free_cells = manager.getMap()->freeCells();
    auto poses = manager.getPositions();

    Mover::Move(&enemy, dx, dy, free_cells, poses);

    Map* map = manager.getMap();
    Position newPos = enemy.getPos();
    if (map->getTile(newPos.x, newPos.y).getType() == TileType::Trap) {
        Fighter::applyDamage(enemy, enemy, 3);
        map->setTile(newPos.x, newPos.y, TileType::Grass);
    }

    Fighter::HandAttack(enemy, manager);
}

void AISystem::TowerTurn(eManager& manager, int turn_counter) {
    Entity* tower = manager.getTower();
    Entity* player = manager.getPlayer();
    if (!tower || !tower->isAlive()) return;
    if (!player || !player->isAlive()) return;

    if (turn_counter % 3 != 0) return;

    DirectDamageSpell spell(2, 2);

    if (Fighter::CastDirectDamage(*tower, manager, spell)) {
        std::cout << "Башня наносит 2 урона игроку!\n";
    }
}

