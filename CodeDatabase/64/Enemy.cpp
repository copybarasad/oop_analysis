#include "Enemy.h"
#include "Player.h"
#include "GameField.h"
#include <cmath>
#include <vector>

Enemy::Enemy(int startX, int startY) 
    : Character(30, 5, startX, startY) {
}

void Enemy::determineNextMove(const Player& player, const GameField& field, 
                             int& nextX, int& nextY) const {
    nextX = coordX;
    nextY = coordY;
    
    int playerX = player.getCoordX();
    int playerY = player.getCoordY();
    
    int xDifference = playerX - coordX;
    int yDifference = playerY - coordY;
    
    if (std::abs(xDifference) > std::abs(yDifference)) {
        if (xDifference > 0) {
            nextX++;
        } else {
            nextX--;
        }
        
        if (!field.isPositionWithinBounds(nextX, nextY) || !field.isCellPassable(nextX, nextY)) {
            nextX = coordX;
            if (yDifference > 0) {
                nextY++;
            } else {
                nextY--;
            }
        }
    } else {
        if (yDifference > 0) {
            nextY++;
        } else {
            nextY--;
        }
        
        if (!field.isPositionWithinBounds(nextX, nextY) || !field.isCellPassable(nextX, nextY)) {
            nextY = coordY;
            if (xDifference > 0) {
                nextX++;
            } else {
                nextX--;
            }
        }
    }
}