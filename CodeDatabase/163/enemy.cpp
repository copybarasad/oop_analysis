#include "enemy.hpp"
#include "map.hpp"
#include "player.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace {
    void initEnemyRandom() {
        static bool initialized = false;
        if (!initialized) {
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
            initialized = true;
        }
    }
}

Enemy::Enemy(int startX, int startY, Cell& cell, int hp, int range, 
             int damage, Player* player)
    : GameObject(startX, startY, cell, hp, range), 
      attackDamage(damage), targetPlayer(player), 
      hasMovedThisTurn(false), hasAttackedThisTurn(false) {
    
    if (!player) {
        throw std::invalid_argument("Врагу нужна цель в виде игрока");
    }
}

int Enemy::getAttackDamage() const {
    return attackDamage;
}

bool Enemy::canAttack() const {
    return !hasAttackedThisTurn && targetPlayer && targetPlayer->isAlive();
}

void Enemy::performAttack(GameObject& target) {
    if (!canAttack()) {
        return; 
    }
    target.takeDamage(attackDamage);
    hasAttackedThisTurn = true;
}

void Enemy::resetAttackFlag() {
    hasAttackedThisTurn = false;
}

int Enemy::calculateDistance(int x1, int y1, int x2, int y2) const {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool Enemy::canAttackPlayer() const {
    if (!targetPlayer || !targetPlayer->isAlive() || !canAttack()) {
        return false;
    }
    
    int distance = calculateDistance(positionX, positionY, 
                                     targetPlayer->getPositionX(), 
                                     targetPlayer->getPositionY());
    return distance <= 1;
}

void Enemy::tryAttackPlayer() {
    if (canAttackPlayer()) {
        performAttack(*targetPlayer);
        std::cout << "Враг в" << positionX << ", " << positionY 
                  << ") атакует игрока на " << attackDamage << " урона!\n";
    }
}

bool Enemy::findBestMove(Map& map, int& outX, int& outY) {
    if (!targetPlayer || !targetPlayer->isAlive()) {
        return false;
    }
    
    int playerX = targetPlayer->getPositionX();
    int playerY = targetPlayer->getPositionY();
    
    if (calculateDistance(positionX, positionY, playerX, playerY) <= 1) {
        return false; 
    }
    
    int directions[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    
    initEnemyRandom();
    int startIndex = (positionX + positionY) % 4;
    
    int bestX = positionX;
    int bestY = positionY;
    int bestDistance = calculateDistance(positionX, positionY, playerX, playerY);
    bool foundMove = false;
    
    for (int i = 0; i < 4; i++) {
        int index = (startIndex + i) % 4;
        int newX = positionX + directions[index][0];
        int newY = positionY + directions[index][1];
        
        if (canMoveTo(newX, newY, map)) {
            int newDistance = calculateDistance(newX, newY, playerX, playerY);
            
            if (newDistance < bestDistance) {
                bestDistance = newDistance;
                bestX = newX;
                bestY = newY;
                foundMove = true;
            }
        }
    }
    
    if (foundMove) {
        outX = bestX;
        outY = bestY;
        return true;
    }
    
    return false;
}

bool Enemy::tryMoveTowardsPlayer(Map& map) {
    if (!targetPlayer || !targetPlayer->isAlive() || hasMovedThisTurn) {
        return false;
    }
    
    int moveX, moveY;
    if (findBestMove(map, moveX, moveY)) {
        if (moveTo(moveX, moveY, map)) {
            hasMovedThisTurn = true;
            std::cout << "Враг переместился в" << moveX << ", " << moveY << ")\n";
            return true;
        }
    }
    
    return false;
}

void Enemy::moveRandomly(Map& map) {
    if (hasMovedThisTurn) {
        return;
    }
    
    int directions[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    
    initEnemyRandom();
    int startIndex = (positionX * positionY + positionX + positionY) % 4;
    
    for (int i = 0; i < 4; i++) {
        int index = (startIndex + i) % 4;
        int newX = positionX + directions[index][0];
        int newY = positionY + directions[index][1];
        
        if (canMoveTo(newX, newY, map)) {
            if (moveTo(newX, newY, map)) {
                hasMovedThisTurn = true;
                std::cout << "Враг переместился в " << newX << ", " << newY << ")\n";
                break;
            }
        }
    }
}

void Enemy::takeTurn(Map& map) {
    if (!isAlive()) {
        return;
    }
    
    resetTurnFlags();
    
    tryAttackPlayer();
    
    if (!hasAttackedThisTurn) {
        if (!tryMoveTowardsPlayer(map)) {
            moveRandomly(map);
        }
        
        tryAttackPlayer();
    }
}

void Enemy::resetTurnFlags() {
    hasMovedThisTurn = false;
    hasAttackedThisTurn = false;
}