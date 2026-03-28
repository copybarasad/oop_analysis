#pragma once

#include "objects/Enemy.h"
#include "objects/GameObject.h"
#include "spells/DirectDamageSpell.h"
#include <memory>
#include <cmath>

class DamageTower : public Enemy {
    int attackRange;
    int cooldown = 0;
    int currentCooldown = 0;
    std::unique_ptr<DirectDamageSpell> spell;
    
public:
    DamageTower(int x, int y, char sprite = 'I', std::string color = "\033[35m",
               int health = 50, int damage = 1, int speed = 0, int aggroRange = 5, 
               ObjectTag tags = ObjectTag::TAG_DAMAGE_TOWER, int attackRange = 4, int cooldown = 3)
        : Enemy(x, y, sprite, color, health, damage, speed, aggroRange, tags),
          attackRange(attackRange), cooldown(cooldown) {
        spell = std::make_unique<DirectDamageSpell>(aggroRange, damage);
    }

    int getAttackRange() const { return attackRange; }
    int getCurrentCooldown() const { return currentCooldown; }
    bool canAttack() const { return currentCooldown == 0; }

    void updateCooldown() {
        if (currentCooldown > 0) {
            currentCooldown--;
        }
    }

    bool tryAttackPlayer(Player* player, Field& field) {
        if (!player || !player->isAlive() || !canAttack()) {
            return false;
        }

        int px = player->getX();
        int py = player->getY();

        int distance = getDistance(x, y, px, py);

        if (distance <= attackRange) {
            if (spell->use(field, nullptr, px, py)) {
                currentCooldown = cooldown;
                return true;
            }
        }
        
        return false;
    }

    std::unique_ptr<GameObject> clone() const override {
        auto tower = std::make_unique<DamageTower>(
            x, y, sprite, color, health, damage, speed, getAggroRange(),
            static_cast<ObjectTag>(tags), attackRange, cooldown);
        tower->setCurrentCooldown(currentCooldown);
        tower->setCanMove(canMove);
        return tower;
    }

private:
    int getDistance(int ax, int ay, int bx, int by) {
        int dx = bx - ax;
        int dy = by - ay;
        return std::sqrt(dx*dx + dy*dy);
    }

    void setCurrentCooldown(int cd) {
        currentCooldown = cd;
    }
};
