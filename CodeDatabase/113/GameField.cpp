#include "GameField.h"
#include "Player.h"

void GameField::triggerTrap(int x, int y, Player& player) {
    if (hasTrap(x, y)) {
        int damage = getTrapDamage(x, y);
        player.takeDamage(damage);
        std::cout << "Trap triggered! " << damage << " damage to player at ("
            << x << "," << y << ")" << std::endl;
        removeTrap(x, y);
    }
}