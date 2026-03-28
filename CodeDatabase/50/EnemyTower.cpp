#include "EnemyTower.h"

EnemyTower::EnemyTower(int x, int y, double damage, int range, int cooldown)
    : GameEnt(20, 0, x, y, GameEntType::Tower),
      spell(damage, range, x, y),
      cooldownTurns(0),
      maxCooldown(cooldown) {}

void EnemyTower::tryAttack(Field& field, Player& player) {
    if (cooldownTurns > 0) {
        std::cout << "Башня (" << getX() << ", " << getY() 
                  << ") перезаряжается (" << cooldownTurns << " ходов осталось).\n";
        cooldownTurns--;
        return;
    }

    spell.cast(field, player);


    cooldownTurns = maxCooldown;
}

void EnemyTower::reduceCooldown() {
    if (cooldownTurns > 0)
        cooldownTurns--;
}
