#include "areaspell.h"
#include "emanager.h"
#include <iostream>

bool AreaSpell::do_spell(int x, int y, Field& field, Player& player, Enemy& enemy, std::string& message) {
    if (!checkResources(x, y, field, player, message)) return false;
    bool hit = false;
    for (int dx = 0; dx < 2; dx++) {
        for (int dy = 0; dy < 2; dy++) {
            int targetX = x + dx;
            int targetY = y + dy;
            if (field.validpos(targetX, targetY)) {
                if (enemy.getX() == targetX && enemy.getY() == targetY && enemy.isAlive()){
                    enemy.takeDamage(damage);
                    hit = true;
                }
            }
        }
    }
    removeCash(player);
    message = name + (hit ? " попал по врагу!" : " не попал по врагу");
    return true;
}