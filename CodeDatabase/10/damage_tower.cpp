#include "damage_tower.h"
#include <iostream>
#include "constants.h"

DamageTower::DamageTower(std::string name, Field& field, int startX, int startY) 
    : name(name), x(startX), y(startY), field(field), 
      lives(constants::DAMAGE_TOWER_LIVES),
      spell(),
      cooldownTimer(0), 
      cooldownDuration(constants::DAMAGE_TOWER_COOLDOWN) {
    
    /*if (!field.isValidPosition(x, y) || !field.getCell(x, y).isEmpty()) {
        throw std::runtime_error("Невозможно разместить башню урона");
    }*/
    field.getCell(x, y).setDamageTower();
}

bool DamageTower::canAttackPlayer(int playerX, int playerY) const {
    if (cooldownTimer > 0 || !isAlive()) {
        return false;
    }
    
    return spell.canCast(playerX, playerY, x, y, field);
}

int DamageTower::attackPlayer(int playerX, int playerY) {
    if (!canAttackPlayer(playerX, playerY)) {
        return 0;
    }

    int damage = spell.cast_spell(playerX, playerY, x, y, field);
    if (damage > 0) {
        cooldownTimer = cooldownDuration;
    }
    return damage;
}

void DamageTower::takeDamage(int damage) {
    if (isAlive()) {
        lives -= damage;
        if (lives <= 0) {
            lives = 0;
            field.getCell(x, y).setEmpty();
            std::cout << name << " разрушена!" << std::endl;
        } else {
            std::cout << name << " получила " << damage << " урона. Осталось жизней: " << lives << std::endl;
        }
    }
}

void DamageTower::updateCooldown() {
    if (cooldownTimer > 0) {
        cooldownTimer--;
    }
}

bool DamageTower::isAlive() const {
    return lives > 0;
}

std::string DamageTower::getName() const {
    return name;
}

int DamageTower::getX() const {
    return x;
}

int DamageTower::getY() const {
    return y;
}

int DamageTower::getLives() const {
    return lives;
}

int DamageTower::getCooldownTimer() const {
    return cooldownTimer;
}

void DamageTower::setLives(int newLives) { lives = newLives; }
void DamageTower::setCooldown(int newCooldown) { cooldownTimer = newCooldown; }
void DamageTower::upgradeDamage(int increment) {
    spell.upgrade(increment);
}
