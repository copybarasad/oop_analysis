#include "DirectDamageSpell.h"
#include "Player.h"
#include "GameField.h"
#include "Unit.h"
#include "Building.h"
#include <cmath>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int damage, int range)
        : damage_(damage), range_(range) {}

bool DirectDamageSpell::use(Player& player, GameField& field, int targetX, int targetY) {
    int px = player.x(), py = player.y();
    int dx = px - targetX;
    int dy = py - targetY;
    if (std::abs(dx) > range_ || std::abs(dy) > range_) {
        std::cout << "DirectDamage: цель вне радиуса.\n";
        return false;
    }

    Unit* u = field.unitAt(targetX, targetY);
    if (u && u->isEnemy()) {
        u->takeDamage(damage_);
        std::cout << "DirectDamage: нанесено " << damage_ << " урона юниту.\n";
        player.spendTurn();
        return true;
    }
    Building* b = field.buildingAt(targetX, targetY);
    if (b && b->isEnemyBuilding()) {
        b->takeDamage(damage_);
        std::cout << "DirectDamage: нанесено " << damage_ << " урона зданию.\n";
        player.spendTurn();
        return true;
    }
    std::cout << "DirectDamage: нет подходящей цели.\n";
    return false;
}
