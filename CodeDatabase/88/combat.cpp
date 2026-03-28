#include "combat.hpp"
#include <iostream>

Combat::Combat(GameMap& map) : gameMap(map) {}

bool Combat::performMeleeAttack(Character* attacker, Character* target) {
    if (attacker == nullptr || target == nullptr) {
        return false;
    }
    int damage = attacker->attack();
    target->takeDamage(damage);
    
    std::cout <<(attacker->getIcon()) << ": Melee attack! " << damage << " damage dealt!\n";
    
    if (!target->isAlive()) {
        handleDefeatedEnemy(target, false);
    }
    
    return true;
}

bool Combat::performRangedAttack(Player* player, Character* target) {
    if (player == nullptr || target == nullptr) {
        return false;
    }
    
    int damage = player->attack() / 2;
    target->takeDamage(damage);
    
    std::cout << "Ranged attack! " << damage << " damage dealt!\n";
    
    if (!target->isAlive()) {
        handleDefeatedEnemy(target, true);
        player->addScore(5);
    }
    
    return true;
}

Character* Combat::findRangedTarget(Player* player, int currentX, int currentY, int directionX, int directionY) {
    if (player == nullptr) {
        return nullptr;
    }
    if (player->getWeapon() != RANGED) {
        return nullptr;
    }
    
    int range = player->getRange();
    
    for (int distance = 1; distance <= range; distance++) {
        int checkX = currentX + (directionX * distance);
        int checkY = currentY + (directionY * distance);
        
        if (!gameMap.isValidPosition(checkX, checkY)) {
            return nullptr;
        }
        
        if (!gameMap.isPositionPassable(checkX, checkY)) {
            return nullptr;
        }
        
        Character* character = gameMap.getCharacterAt(checkX, checkY);
        if (character && dynamic_cast<Enemy*>(character)) {
            return character;
        }
    }
    
    return nullptr;
}

void Combat::handleDefeatedEnemy(Character* enemy, bool isRanged) {
    if (enemy == nullptr) {
        return;
    }
    int x = enemy->getX();
    int y = enemy->getY();
    gameMap.removeCharacter(x, y);
    
    std::cout << "Enemy defeated" << "!\n";
}