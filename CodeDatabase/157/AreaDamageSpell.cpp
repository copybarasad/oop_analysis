#include "AreaDamageSpell.h"
#include "Player.h"
#include "GameField.h"
#include "Unit.h"
#include "Building.h"
#include <cmath>
#include <iostream>

AreaDamageSpell::AreaDamageSpell(int damage, int range)
        : damage_(damage), range_(range) {}

bool AreaDamageSpell::use(Player& player, GameField& field, int targetX, int targetY) {
    int px = player.x();
    int py = player.y();

    if (std::abs(px - targetX) > range_ || std::abs(py - targetY) > range_) {
        std::cout << "AreaDamage: цель вне радиуса.\n";
        return false;
    }

    // 2. Проверка валидности целевой клетки
    // Если мы пытаемся скастовать заклинание за карту, это ошибка.
    if (!field.isInside(targetX, targetY)) {
        std::cout << "AreaDamage: цель вне поля.\n";
        return false;
    }

    bool hitSomething = false;

    // 3. Проходим по области 2x2
    // (targetX, targetY) - это левый верхний угол области поражения
    for (int dx = 0; dx < 2; ++dx) {
        for (int dy = 0; dy < 2; ++dy) {
            int x = targetX + dx;
            int y = targetY + dy;

            // Проверяем, что конкретная клетка области находится внутри поля
            if (field.isInside(x, y)) {
                // Наносим урон юнитам
                Unit* u = field.unitAt(x, y);
                if (u && u->isEnemy()) {
                    u->takeDamage(damage_);
                    std::cout << "AreaDamage: нанесено " << damage_ << " урона юниту в (" << x << "," << y << ")\n";
                    hitSomething = true;
                }

                // Наносим урон зданиям
                Building* b = field.buildingAt(x, y);
                if (b && b->isEnemyBuilding()) {
                    b->takeDamage(damage_);
                    std::cout << "AreaDamage: нанесено " << damage_ << " урона зданию в (" << x << "," << y << ")\n";
                    hitSomething = true;
                }
            }
        }
    }

    if (!hitSomething) {
        std::cout << "AreaDamage: заклинание использовано в (" << targetX << "," << targetY << "), но никого не задело.\n";
    }

    // 4. Тратим ход и возвращаем true, так как заклинание сработало (даже если впустую)
    player.spendTurn();
    return true;
}