#pragma once

#include "Field.h"
#include "objects/GameObject.h"
#include "objects/Enemy.h"
#include "InputManager.h"

class CollisionSystem {
public:
    bool tryMovePlayer(Field& field, int dx, int dy) {
        Player* player = field.getPlayer();
        if (!player) return false;

        int newX = player->getX() + dx;
        int newY = player->getY() + dy;

        if (!field.isInBounds(newX, newY)) {
            return false;
        }

        GameObject* obj = field.getObjectAt(newX, newY);

        if (player->getCanMove() != 0) {
            return false;
        }
        
        if (!obj) {
            player->move(dx, dy);
            return true;
        }

        if (obj->hasTag(ObjectTag::TAG_SLOW_CELL)) {
            player->setCanMove(1);
            player->move(dx, dy);
            return true;
        }

        if (obj->hasTag(ObjectTag::TAG_ENEMY) || obj->hasTag(ObjectTag::TAG_TOWER)) {
            Enemy* enemy = static_cast<Enemy*>(obj);
            player->attack(enemy);
            return false;
        }

        if (obj->isSolid()) {
            return false;
        }

        player->move(dx, dy);
        return true;
    }

    bool canMoveTo(Field& field, int x, int y) const {
        GameObject* obj = field.getObjectAt(x, y);

        if (!obj) {
            return true;
        }

        if (obj->isSolid()) {
            return false;
        }

        return true;
    }
#if 0
    void tryAttackRange(Field& field, Direction direction) {
        Player* player = field.getPlayer();
        int px = player->getX();
        int py = player->getY();

        GameObject* obj;
        bool found = false;
        switch (direction) {
        case Direction::RIGHT:
            for (int i = px + 1; i <= px + player->getRange(); i++) {
                obj = field.getObjectAt(i, py);
                if (!obj || !obj->hasTag(ObjectTag::TAG_ENEMY) && !obj->hasTag(ObjectTag::TAG_TOWER)) {
                    continue; 
                }
                found = true;
                break;
            }
            break;
        case Direction::LEFT:
            for (int i = px - 1; i >= px - player->getRange(); i--) {
                obj = field.getObjectAt(i, py);
                if (!obj || !obj->hasTag(ObjectTag::TAG_ENEMY) && !obj->hasTag(ObjectTag::TAG_TOWER)) {
                    continue;
                }
                found = true;
                break;
            }
            break;
        case Direction::UP:
            for (int i = py - 1; i >= py - player->getRange(); i--) {
                obj = field.getObjectAt(px, i);
                if (!obj || !obj->hasTag(ObjectTag::TAG_ENEMY) && !obj->hasTag(ObjectTag::TAG_TOWER)) {
                    continue;
                }
                found = true;
                break;
            }
            break;
        case Direction::DOWN:
            for (int i = py + 1; i < py + player->getRange(); i++) {
                obj = field.getObjectAt(px, i);
                if (!obj || !obj->hasTag(ObjectTag::TAG_ENEMY) && !obj->hasTag(ObjectTag::TAG_TOWER)) {
                    continue;
                }
                found = true;
                break;
            }
            break;
        case Direction::NONE:
        default:
            return;
        }
        if (found) {
            Enemy* enemy = static_cast<Enemy*>(obj);
            enemy->takeDamage(player->getRangeDamage());
        }
    }
#endif
};
