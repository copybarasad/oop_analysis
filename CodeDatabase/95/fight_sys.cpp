#include "fight_sys.hpp"
#include "game/entity_manager.hpp"
#include "game_objects/entity.hpp"
#include "game_objects/spells/damage_spell.hpp"
#include "game_objects/spells/aoe_spell.hpp"
#include "game_objects/spells/trap_spell.hpp"

#include <iostream>

void Fighter::applyDamage(Entity& source, Entity& target, int dmg) {
    target.takeDamage(dmg);
}

void Fighter::HandAttack(Entity& attacker, eManager& manager) {
    if (!attacker.isAlive()) return;

    Position ap = attacker.getPos();

    const int dirs[4][2] = {
        { 1,  0 },
        { -1, 0 },
        { 0,  1 },
        { 0, -1 }
    };

    auto entities = manager.getEntities();

    for (auto& d : dirs) {
        int tx = ap.x + d[0];
        int ty = ap.y + d[1];

        for (Entity* target : entities) {
            if (!target) continue;
            if (!target->isAlive()) continue;

            if (target == &attacker) continue;

            Position tp = target->getPos();
            if (tp.x == tx && tp.y == ty) {

                if (target->getFaction() != attacker.getFaction()) {
                    applyDamage(attacker, *target, attacker.getDamage());
                    return;
                }
            }
        }
    }
}

bool Fighter::CastDirectDamage(Entity& caster, eManager& manager, DirectDamageSpell& spell) {
    auto casterPos = caster.getPos();
    const Map* map = manager.getMap();
    bool applied = false;

    for (Entity* e : manager.getEntities()) {
        if (!e->isAlive() || e == &caster) continue;
        if (caster.getFaction() == e->getFaction()) continue;

        Position pos = e->getPos();

        int dx = abs(pos.x - casterPos.x);
        int dy = abs(pos.y - casterPos.y);

        if (dx <= spell.getRange() && dy <= spell.getRange()) {
            applyDamage(caster, *e, spell.getDamage());
            applied = true;
        }
    }

    if (!applied) {
        std::cout << "В радиусе заклинания нет врагов.\n";
    }

    return applied;
}

bool Fighter::CastAoeDamage(Entity& caster, eManager& manager, AoeDamageSpell& spell, int startX, int startY) {
    Map* map = manager.getMap();
    int endX = startX + 1;
    int endY = startY + 1;

    if (startX < 0 || startY < 0 || endX >= map->getWidth() || endY >= map->getHeight()) {
        std::cout << "Неверные координаты для АОЕ заклинания!\n";
        return false;
    }

    for (Entity* e : manager.getEntities()) {
        if (!e->isAlive() || e == &caster) continue;
        if (caster.getFaction() == e->getFaction()) continue;

        Position pos = e->getPos();
        if (pos.x >= startX && pos.x <= endX && pos.y >= startY && pos.y <= endY) {
            applyDamage(caster, *e, spell.getDamage());
        }
    }

    return true;
}

bool Fighter::CastTrap(Entity& caster, eManager& manager, TrapSpell& spell, int targetX, int targetY) {
    Map* map = manager.getMap();
    Position casterPos = caster.getPos();

    if (targetX < 0 || targetY < 0 || targetX >= map->getWidth() || targetY >= map->getHeight()) {
        std::cout << "Неверные координаты для ловушки!\n";
        return false;
    }

    if (map->getTile(targetX, targetY).getType() == TileType::Wall) {
        std::cout << "Невозможно поставить ловушку на стену!\n";
        return false;
    }

    int dx = abs(targetX - casterPos.x);
    int dy = abs(targetY - casterPos.y);
    if (dx > spell.getRange() || dy > spell.getRange()) {
        std::cout << "Координаты ловушки слишком далеко!\n";
        return false;
    }

    map->setTile(targetX, targetY, TileType::Trap);
    return true;
}
