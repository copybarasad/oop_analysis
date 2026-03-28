#include "EnemyTower.h"

EnemyTower::EnemyTower(int health, int startX, int startY, char symbol) : 
                    health(health), posX(startX), posY(startY), symbol(symbol), attackCooldown(0), attackDelay(3) {
    attackSpell = new DirectDamageSpell(10, 3);  // создаем заклинание для башни
}

// Атаковать игрока если находится в диапозоне действия заклинания
void EnemyTower::attackPlayerIfRange(Player& player, GameState& state) {
    if (!isAlive()) return;

    if (attackCooldown > 0) {
        attackCooldown--;
        return;
    }

    int dx = std::abs(player.getX() - posX);
    int dy = std::abs(player.getY() - posY);

    if (dx <= attackSpell->getRange() && dy <= attackSpell->getRange()) {
        attackSpell->useOnPlayer(player, state);
        attackCooldown = attackDelay;
    }
}

// Боёвка
void EnemyTower::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

// Геттеры
bool EnemyTower::isAlive() const { return health > 0; }

int EnemyTower::getHealth() const { return health; }

int EnemyTower::getX() const { return posX; }

int EnemyTower::getY() const { return posY; }

char EnemyTower::getSymbol() const { return symbol; }