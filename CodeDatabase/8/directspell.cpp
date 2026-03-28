#include "directspell.h"
#include "emanager.h"
#include <iostream>
#include <cmath>
#include <algorithm>

bool DirectSpell::do_spell(int x, int y, Field& field, Player& player, Enemy& enemy, std::string& message) {
    if (!checkResources(x, y, field, player, message)) return false;
    if (x == enemy.getX() && y == enemy.getY() && enemy.isAlive()) {
        enemy.takeDamage(damage);
        removeCash(player);
        message = name + " наносит " + std::to_string(damage) + " урона " + enemy.getname();
        return true;
    } else {
        message = "В целевой клетке нет врага!";
        return false;
    }
}