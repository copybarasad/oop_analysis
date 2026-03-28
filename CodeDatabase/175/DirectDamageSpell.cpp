#include "DirectDamageSpell.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include <cmath>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int damage, int radius, int mult)
    : baseDamage_(damage), radius_(radius), damageMultiplier_(mult) {}

std::string DirectDamageSpell::getName() const {
    return "Direct Damage";
}

bool DirectDamageSpell::apply(Field& field, const Player& player, int targetX, int targetY) {
    auto [px, py] = field.findPlayer();
    double dist = std::hypot(targetX - px, targetY - py);
    if (dist > radius_) return false;

    auto& cell = field.getCell(targetX, targetY);
    int totalDamage = baseDamage_ * damageMultiplier_;

    if (auto enemy = cell.getEnemy()) {
        if (enemy->isAlive()) {
            enemy->takeDamage(totalDamage);
            std::cout << "Заклинание нанесло " << totalDamage << " урона врагу!" << std::endl;
            if (!enemy->isAlive()) {
                std::cout << "Враг уничтожен заклинанием!" << std::endl;
            }
            return true;
        }
    }
    if (auto base = cell.getBase()) {
        if (base->isAlive()) {
            base->takeDamage(totalDamage);
            std::cout << "Заклинание нанесло " << totalDamage << " урона базе! HP: " 
                      << base->getHp() << "/" << base->getMaxHp() << std::endl;
            if (!base->isAlive()) {
                std::cout << "База уничтожена заклинанием!" << std::endl;
            }
            return true;
        }
    }
    if (auto tower = cell.getTower()) {
        if (tower->isAlive()) {
            tower->takeDamage(totalDamage);
            std::cout << "Заклинание нанесло " << totalDamage << " урона башне! HP: " 
                      << tower->getHp() << "/" << tower->getMaxHp() << std::endl;
            if (!tower->isAlive()) {
                std::cout << "Башня уничтожена заклинанием!" << std::endl;
            }
            return true;
        }
    }
    return false;
}