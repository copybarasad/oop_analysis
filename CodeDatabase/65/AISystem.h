#pragma once

#include "Field.h"
#include "systems/CollisionSystem.h"
#include "objects/GameObject.h"
#include "objects/DamageTower.h"
#include "objects/Tower.h"
#include <cmath>
#include "objects/Trap.h"
#include <memory>

class AISystem {
public:
    void updateEnemies(Field& field, const CollisionSystem& collison) {
        Player* player = field.getPlayer();
        if (!player || !player->isAlive()) return;

        int px = player->getX();
        int py = player->getY();

        const std::vector<std::unique_ptr<GameObject>>& objects = field.getObjects();
        int size = objects.size();
        for (int i = 0; i < size; i++) {
            std::unique_ptr<GameObject> const & obj = objects[i];
            if (obj && obj->hasTag(ObjectTag::TAG_TOWER)) {
                Tower* tower = static_cast<Tower*>(obj.get());
                if (!tower->isAlive()) continue;
                updateTower(tower, field);
                continue;
            }

            if (obj && obj->hasTag(ObjectTag::TAG_DAMAGE_TOWER)) {
                DamageTower* damageTower = static_cast<DamageTower*>(obj.get());
                if (!damageTower->isAlive()) continue;
                updateDamageTower(damageTower, player, field);
                continue;
            }

            if (obj && !obj->hasTag(ObjectTag::TAG_ENEMY)) continue;

            Enemy* enemy = static_cast<Enemy*>(obj.get());
            if (!enemy->isAlive()) continue;

            updateEnemy(enemy, px, py, field, collison);
        }
    }
private:
    int sgn(int a) { return a > 0 ? 1 : (a < 0 ? -1 : 0); }
    void updateEnemy(Enemy* enemy, int playerX, int playerY,
                     Field& field, const CollisionSystem& collison) {
        int ex = enemy->getX();
        int ey = enemy->getY();


        auto objectsAtPos = field.getObjectsAt(ex, ey);
        for (auto obj : objectsAtPos) {
            if (obj->hasTag(ObjectTag::TAG_TRAP)) {
                Trap* trap = static_cast<Trap*>(obj);
                if (!trap->isActivated()) {
                    trap->activate(enemy);
                    break;
                }
            }
        }

        int distance = getDistance(ex, ey, playerX, playerY);

        if (distance > enemy->getAggroRange()) {
            return;
        }

        if (isNearTo(ex, ey, playerX, playerY)) {
            Player *player = field.getPlayer();
            enemy->attack(player);
            return;
        }

        moveTowards(enemy, playerX, playerY, field, collison);
    }

    void updateTower(Tower* tower, Field& field) {
        if (tower->getCurrentStep() != tower->getStepBeforeSpawn()) {
            tower->setCurrentStep(tower->getCurrentStep() + 1);
            return;
        }

        for (int y = tower->getY() - tower->getAggroRange(); y <= tower->getY() + tower->getAggroRange(); y++) {
            for (int x = tower->getX() - tower->getAggroRange(); x <= tower->getX() + tower->getAggroRange(); x++) {
                if (x == tower->getX() && y == tower->getY()) continue;
                GameObject* obj = field.getObjectAt(x, y);
                if (!obj && field.isInBounds(x, y)) {
                    field.addObject(std::make_unique<Enemy>(x, y));
                    tower->setCurrentStep(1);
                    return;
                }
            }
        }

    }

    int getDistance(int ax, int ay, int bx, int by) {
        int dx = bx - ax;
        int dy = by - ay;
        return std::sqrt(dx*dx + dy*dy);
    }

    bool isNearTo(int x1, int y1, int x2, int y2) const {
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        return (dx <= 1 && dy <= 1 && (dx + dy) > 0);
    }

    void updateDamageTower(DamageTower* tower, Player* player, Field& field) {
        tower->updateCooldown();
        tower->tryAttackPlayer(player, field);
    }

    void moveTowards(Enemy* enemy, int targetX, int targetY,
                     Field& field, const CollisionSystem& collison) {
        int ex = enemy->getX();
        int ey = enemy->getY();

        int diffX = std::abs(targetX - ex);
        int diffY = std::abs(targetY - ey);

        int dx = 0;
        int dy = 0;

        if (diffX > diffY) {
            dx = sgn(targetX - ex);
        } else if (diffY > diffX) {
            dy = sgn(targetY - ey);
        } else if (diffX > 0) {
            dx = sgn(targetX - ex);
        }

        int newX = ex + dx;
        int newY = ey + dy;

        if (collison.canMoveTo(field, newX, newY)) {
            enemy->move(dx, dy);
        } else if (dx != 0 && dy == 0) {
            dy = sgn(targetY - ey);
            newX = ex;
            newY = ey + dy;
            if (collison.canMoveTo(field, newX, newY)) {
                enemy->move(0, dy);
            }
        } else if (dy != 0 && dx == 0) {
            dx = sgn(targetX - ex);
            newX = ex + dx;
            newY = ey;
            if (collison.canMoveTo(field, newX, newY)) {
                enemy->move(dx, 0);
            }
        }
    }
};
