#include "EnemyTower.h"
#include "spells/DirectDamageSpell.h"
#include "Game.h"
#include "Exceptions.h"

namespace Game {

EnemyTower::EnemyTower(Utils::Point position, int health, int damage, int range, int cooldown)
    : position(position),
      health(health),
      damage(damage),
      attackRange(range),
      attackCooldown(cooldown),
      currentCooldown(0)
{}

EnemyTower::EnemyTower(std::istream& is) {
    deserialize(is);
}

const Utils::Point& EnemyTower::getPosition() const { return position; }
int EnemyTower::getHealth() const { return health; }
int EnemyTower::getDamage() const { return damage; }
int EnemyTower::getAttackRange() const { return attackRange; }
bool EnemyTower::isAlive() const { return health > 0; }

void EnemyTower::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool EnemyTower::canAttack() const {
    return currentCooldown <= 0;
}

void EnemyTower::attack(Game& game) {
    DirectDamageSpell weakenedSpell(this->damage, this->attackRange);
    
    //std::cout << "Tower at (" << this->position.row << ", " << this->position.column << ") tries to cast a spell on the player." << std::endl;
    bool success = weakenedSpell.cast(game, this->position, game.getPlayer().getPosition());

    if (success) {
        currentCooldown = attackCooldown;
    }
}

void EnemyTower::tickCooldown() {
    if (currentCooldown > 0) {
        currentCooldown--;
    }
}

void EnemyTower::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&position), sizeof(position));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    os.write(reinterpret_cast<const char*>(&attackRange), sizeof(attackRange));
    os.write(reinterpret_cast<const char*>(&attackCooldown), sizeof(attackCooldown));
    os.write(reinterpret_cast<const char*>(&currentCooldown), sizeof(currentCooldown));
}

void EnemyTower::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&position), sizeof(position));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    is.read(reinterpret_cast<char*>(&attackRange), sizeof(attackRange));
    is.read(reinterpret_cast<char*>(&attackCooldown), sizeof(attackCooldown));
    is.read(reinterpret_cast<char*>(&currentCooldown), sizeof(currentCooldown));
    if (is.fail()) throw FileReadException("Failed to read EnemyTower data.");
}

}