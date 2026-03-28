#include "Enemy.h"
#include "Player.h"
#include "GameField.h"
#include <cstdlib>
#include <vector>
#include <cmath>

Enemy::Enemy(int health, int damage, const Position& startPos)
    : health(health), damage(damage), position(startPos) {}

void Enemy::moveTowards(const Position& targetPos, const GameField& field) {
    Position newPos = position;
    
    // Try to move horizontally first
    if (targetPos.x > position.x) {
        newPos.x++;
    } else if (targetPos.x < position.x) {
        newPos.x--;
    }
    
    // Check if horizontal move is valid
    if (field.isCellPassable(newPos) && newPos != position) {
        position = newPos;
        return;
    }
    
    // If horizontal move failed, try vertical
    newPos = position;
    if (targetPos.y > position.y) {
        newPos.y++;
    } else if (targetPos.y < position.y) {
        newPos.y--;
    }
    
    // Check if vertical move is valid
    if (field.isCellPassable(newPos) && newPos != position) {
        position = newPos;
        return;
    }
    
    // If both direct moves failed, try alternative paths
    std::vector<Position> possibleMoves = {
        {position.x + 1, position.y},
        {position.x - 1, position.y},
        {position.x, position.y + 1},
        {position.x, position.y - 1}
    };
    
    // Find the first valid move that gets closer to player
    for (const auto& move : possibleMoves) {
        if (field.isCellPassable(move)) {
            // Check if this move gets us closer to target
            int currentDistance = std::abs(position.x - targetPos.x) + std::abs(position.y - targetPos.y);
            int newDistance = std::abs(move.x - targetPos.x) + std::abs(move.y - targetPos.y);
            
            if (newDistance < currentDistance) {
                position = move;
                return;
            }
        }
    }
}

void Enemy::attack(Player& player) const {
    player.takeDamage(damage);
}