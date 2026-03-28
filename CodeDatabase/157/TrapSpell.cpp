#include "TrapSpell.h"
#include "Player.h"
#include "GameField.h"
#include <cmath>
#include <iostream>

TrapSpell::TrapSpell(int damage, int range)
        : damage_(damage), range_(range) {}

bool TrapSpell::use(Player& player, GameField& field, int targetX, int targetY) {
    int px = player.x(), py = player.y();
    if (std::abs(px - targetX) > range_ || std::abs(py - targetY) > range_) {
        std::cout << "Trap: цель вне радиуса.\n";
        return false;
    }
    if (!field.isInside(targetX, targetY)) {
        std::cout << "Trap: вне поля.\n";
        return false;
    }
    if (field.placeTrap(targetX, targetY, damage_)) {
        std::cout << "Trap: установлена ловушка в (" << targetX << "," << targetY << ")\n";
        player.spendTurn();
        return true;
    } else {
        std::cout << "Trap: не удалось установить ловушку (клетка занята).\n";
    }
    return false;
}
