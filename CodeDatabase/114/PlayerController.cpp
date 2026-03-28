#include "PlayerController.h"
#include "../models/events/EventNotifier.h"
#include "../models/events/PlayerMoveEvent.h"
#include "../models/events/DamageEvent.h"
#include "../models/events/EntityDeathEvent.h"
#include <cmath>
#include <iostream>
#include "Cell.h"

#define PLAYER_SALARY 50

bool PlayerController::isAlive() {
    return field->getPlayer().isAlive();
}

bool PlayerController::movePlayer(int dx, int dy) {
    Player& player = field->getPlayer();
    if (!player.getCanMove()) {
        player.setCanMove(true);
        return true;
    }
    int oldX = player.getX();
    int oldY = player.getY();
    int nx = oldX + dx;
    int ny = oldY + dy;
    if (!field->isValidPosition(nx, ny)) return false;

    if (field->getCells()[ny][nx].getHasEntity()) {
        auto entityOpt = field->findEntityAt(nx, ny);
        if (entityOpt.has_value()) {
            Entity& entity = entityOpt.value().get();
            // Проверяем, что это враг (у врага getDamage() > 0, у здания = 0)
            if (entity.getDamage() > 0) {
                int damage = entity.getDamage();
                // Уведомляем о нанесении урона врагом игроку
                DamageEvent damageEvent("Враг", "Игрок", damage, player.getX(), player.getY());
                EventNotifier::getInstance().notify(damageEvent);
                player.takeDamage(damage);
                return false;
            }
        }
    }
    if (!field->isPositionPassable(nx, ny)) return false;

    field->getCells()[oldY][oldX].setHasEntity(false);
    field->getCells()[ny][nx].setHasEntity(true);
    player.setPosition(nx, ny);

    // Уведомляем о перемещении игрока
    PlayerMoveEvent moveEvent(oldX, oldY, nx, ny);
    EventNotifier::getInstance().notify(moveEvent);

    if (field->getCells()[ny][nx].getType() == CellType::SLOW) {
        player.setCanMove(false);
    }
    return true;
}

bool PlayerController::playerAttack(int tx, int ty) {
    Player& player = field->getPlayer();
    int dx = std::abs(player.getX() - tx);
    int dy = std::abs(player.getY() - ty);
    int manhattan = dx + dy;
    
    auto targetOpt = field->findEntityAt(tx, ty);
    if (!targetOpt.has_value()) return false;
    
    Entity& target = targetOpt.value().get();
    
    if ((player.getCombatMode() == CombatMode::MELEE && manhattan == 1) ||
        (player.getCombatMode() == CombatMode::RANGED && manhattan > 0 && manhattan <= 2)) {
        int damage = player.getDamage();
        std::string targetName = target.getEntityName();
        // Уведомляем о нанесении урона игроком
        DamageEvent damageEvent("Игрок", targetName, damage, tx, ty);
        EventNotifier::getInstance().notify(damageEvent);
        target.takeDamage(damage);
        if (!target.isAlive()) {
            field->getCells()[ty][tx].setHasEntity(false);
            player.setMoney(player.getMoney() + PLAYER_SALARY);
            
            // Уведомляем о смерти сущности
            EntityDeathEvent deathEvent(targetName, tx, ty);
            EventNotifier::getInstance().notify(deathEvent);
            
            // Находим и удаляем из соответствующего контейнера
            auto& enemies = field->getEnemies();
            for (auto it = enemies.begin(); it != enemies.end(); ++it) {
                if (&(*it) == &target) {
                    enemies.erase(it);
                    return true;
                }
            }
            auto& buildings = field->getBuilding();
            for (auto it = buildings.begin(); it != buildings.end(); ++it) {
                if (&(*it) == &target) {
                    buildings.erase(it);
                    return true;
                }
            }
        }
        return true;
    }
    return false;
}

Player& PlayerController::getPlayer() {
    return field->getPlayer();
}
