#include "tower.h"
#include "player.h"
#include "enemy.h"
#include "concrete_spells.h"
#include <iostream>
#include <cmath>

Tower::Tower(int x, int y, int attackRange, int cooldown)
    : x(x), y(y), attackRange(attackRange), 
      cooldown(cooldown), currentCooldown(0), active(true) {
    createAttackSpell();
}

void Tower::createAttackSpell() {
    attackSpell = std::make_unique<TowerBoltSpell>();
}

bool Tower::isPlayerInRange(const Player& player) const {
    if (!active) return false;
    
    int distance = std::max(std::abs(x - player.getX()), std::abs(y - player.getY()));
    return distance <= attackRange;
}

void Tower::update(Player& player, std::vector<Enemy>& enemies) {
    if (!active) return;
    
    if (currentCooldown > 0) {
        currentCooldown--;
    }
    
    if (canAttack() && isPlayerInRange(player)) {
        attack(player, enemies);
        currentCooldown = cooldown;
    }
}

bool Tower::canAttack() const {
    return active && currentCooldown == 0;
}

void Tower::attack(Player& player, std::vector<Enemy>& enemies) {
    if (!attackSpell) return;
    
    std::cout << "Tower at (" << x << ", " << y << ") attacks player!\n";
    attackSpell->cast(x, y, player.getX(), player.getY(), enemies, player);
}