#include "CombatManager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBarrack.h"
#include "Constants.h"
#include <iostream>
#include <string>

using namespace std;

bool CombatManager::performPlayerAttack(Player& player, 
                                      vector<shared_ptr<Enemy>>& enemies,
                                      vector<shared_ptr<EnemyBuilding>>& buildings) const {
    bool attacked = false;
    int range = player.getAttackRange();
    
    for (auto& enemy : enemies) {
        if (tryAttackEnemy(player, enemy, range)) {
            attacked = true;
            break;
        }
    }
    
    if (!attacked) {
        for (auto& building : buildings) {
            if (tryAttackBuilding(player, building, range)) {
                attacked = true;
                break;
            }
        }
    }
    
    if (!attacked) {
        if (player.getFightingStyle() == 'm') {
            cout << "No enemies in range! Attack in 3x3 square around you." << endl;
        } else {
            cout << "No enemies in range! Your bow range is " << player.getAttackRange() 
                 << " cells in all directions." << endl;
        }
    }
    
    return attacked;
}

bool CombatManager::resolveEnemyAttacks(Player& player, const vector<shared_ptr<Enemy>>& enemies) const {
    bool anyEnemyAttacked = false;
    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive() && enemy->canAttackTarget(player.getPosition())) {
            player.takeDamage(enemy->getDamage());
            cout << "Enemy attacked you! Lost " << enemy->getDamage() << " HP." << endl;
            anyEnemyAttacked = true;
        }
    }
    return anyEnemyAttacked;
}

bool CombatManager::tryAttackEnemy(Player& player, shared_ptr<Enemy>& enemy, int range) const {
    if (!enemy || !enemy->isAlive()) {
        return false;
    }
    
    if (isInAttackRange(player.getPosition(), enemy->getPosition(), range)) {
        bool wasAlive = enemy->isAlive();
        enemy->takeDamage(player.getDamage());
        
        string attackType = (player.getFightingStyle() == 'm') ? "Melee" : "Ranged";
        cout << attackType << " attack! You hit Enemy at (" 
             << enemy->getPosition().getX() << "," << enemy->getPosition().getY() 
             << ") for " << player.getDamage() << " damage!" << endl;
        
        if (!enemy->isAlive() && wasAlive) {
            player.addScorePoints(DEFAULT_ENEMY_POINTS);
            cout << "Enemy destroyed! +" << DEFAULT_ENEMY_POINTS << " points." << endl;
        }
        return true;
    }
    
    return false;
}

bool CombatManager::tryAttackBuilding(Player& player, shared_ptr<EnemyBuilding>& building, int range) const {
    if (!building || !building->isAlive()) {
        return false;
    }
    
    if (isInAttackRange(player.getPosition(), building->getPosition(), range)) {
        bool wasAlive = building->isAlive();
        building->takeDamage(player.getDamage());
        
        string attackType = (player.getFightingStyle() == 'm') ? "Melee" : "Ranged";
        cout << attackType << " attack! You hit Enemy building at (" 
             << building->getPosition().getX() << "," << building->getPosition().getY() 
             << ") for " << player.getDamage() << " damage!" << endl;
        
        if (!building->isAlive() && wasAlive) {
            player.addScorePoints(DEFAULT_BUILDING_POINTS);
            cout << "Enemy building destroyed! +" << DEFAULT_BUILDING_POINTS << " points." << endl;
        }
        return true;
    }
    
    return false;
}

bool CombatManager::isInAttackRange(const Position& attackerPos, const Position& targetPos, int range) const {
    int dx = abs(attackerPos.getX() - targetPos.getX());
    int dy = abs(attackerPos.getY() - targetPos.getY());
    return dx <= range && dy <= range && (dx > 0 || dy > 0);
}