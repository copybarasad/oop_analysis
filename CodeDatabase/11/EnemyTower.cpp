#include "EnemyTower.h"
#include "Player.h"
#include "Field.h"
#include "Logger.h"
#include "GameController.h"
#include "FireballSpell.h"
#include <cmath>


EnemyTower::EnemyTower(int startX, int startY, int hp)
    : x(startX), y(startY), health(hp), cooldown(2), turnsUntilAttack(0) {}

bool EnemyTower::isAlive() const { return health > 0; }
int EnemyTower::getX() const { return x; }
int EnemyTower::getY() const { return y; }
int EnemyTower::getHealth() const { return health; }

void EnemyTower::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void EnemyTower::processTurn(Player& player, GameController& game) {
    if (turnsUntilAttack > 0) {
        turnsUntilAttack--;
        return;
    }
    const int towerDamage = 15; 
    const int towerRange = 6;
    double dist = std::hypot(player.getX() - x, player.getY() - y);

    if (dist <= towerRange) {
        FireballSpell weakFireball(towerDamage, towerRange);
        if (weakFireball.castByTower(this->x, this->y, player.getX(), player.getY(), game)) {
            turnsUntilAttack = cooldown;
        }
    }
}