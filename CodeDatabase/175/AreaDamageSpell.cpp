#include "AreaDamageSpell.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(int damage, int size, int mult)
    : baseDamage_(damage), areaSize_(size), sizeMultiplier_(mult) {}

std::string AreaDamageSpell::getName() const {
    return "Area Damage";
}

bool AreaDamageSpell::apply(Field& field, const Player& player, int targetX, int targetY) {
    int actualSize = areaSize_ + sizeMultiplier_;
    int half = actualSize / 2;
    bool hitSomething = false;
    
    for (int dy = -half; dy <= half; ++dy) {
        for (int dx = -half; dx <= half; ++dx) {
            int nx = targetX + dx, ny = targetY + dy;
            if (field.inBounds(nx, ny)) {
                auto& cell = field.getCell(nx, ny);
                int totalDamage = baseDamage_;
                
                if (auto enemy = cell.getEnemy()) {
                    if (enemy->isAlive()) {
                        enemy->takeDamage(totalDamage);
                        hitSomething = true;
                        std::cout << "Область нанесла " << totalDamage << " урона врагу!" << std::endl;
                        if (!enemy->isAlive()) {
                            cell.setEnemy(nullptr);
                            std::cout << "Враг уничтожен!" << std::endl;
                        }
                    }
                }
                if (auto base = cell.getBase()) {
                    if (base->isAlive()) {
                        base->takeDamage(totalDamage);
                        hitSomething = true;
                        std::cout << "Область нанесла " << totalDamage << " урона базе!" << std::endl;
                        if (!base->isAlive()) {
                            cell.setBase(nullptr);
                            std::cout << "База уничтожена!" << std::endl;
                        }
                    }
                }
                if (auto tower = cell.getTower()) {
                    if (tower->isAlive()) {
                        tower->takeDamage(totalDamage);
                        hitSomething = true;
                        std::cout << "Область нанесла " << totalDamage << " урона башне!" << std::endl;
                        if (!tower->isAlive()) {
                            cell.setTower(nullptr);
                            std::cout << "Башня уничтожена!" << std::endl;
                        }
                    }
                }
            }
        }
    }
    
    return hitSomething;
}