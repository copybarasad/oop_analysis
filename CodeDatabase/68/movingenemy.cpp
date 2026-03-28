#include "movingenemy.h"
#include "gamefield.h"
#include "player.h"
#include <iostream>
#include <cmath>

MovingEnemy::MovingEnemy(Position pos, int health, int damage) 
    : position(pos), health(health), damage(damage), alive(true) {}

void MovingEnemy::takeTurn(Player* player, GameField* field) {
    if (!alive || !player || !field) return;
    
    Position playerPos = player->getPosition();
    
    // Check if adjacent to player (can attack)
    int dx = abs(position.x - playerPos.x);
    int dy = abs(position.y - playerPos.y);
    
    if (dx <= 1 && dy <= 1) {  // Adjacent (8 directions)
        // Attack player (like Lab 1)
        std::cout << "Enemy at (" << position.x << ", " << position.y 
                  << ") attacks player for " << damage << " damage!\n";
        player->takeDamage(damage);
        return;
    }
    
    // Otherwise move toward player
    moveToward(playerPos, field);
}

void MovingEnemy::moveToward(Position playerPos, GameField* field) {
    Position newPos = position;
    
    // Simple movement logic: prioritize X or Y direction
    if (abs(playerPos.x - position.x) > abs(playerPos.y - position.y)) {
        // Move horizontally
        if (playerPos.x > position.x) newPos.x++;
        else if (playerPos.x < position.x) newPos.x--;
    } else {
        // Move vertically
        if (playerPos.y > position.y) newPos.y++;
        else if (playerPos.y < position.y) newPos.y--;
    }
    
    // Check if move is valid (not blocked by tower/wall)
    if (field->isValidPosition(newPos) && !field->hasBuildingAt(newPos)) {
        // Also check for traps (optional)
        position = newPos;
        std::cout << "Enemy moves to (" << position.x << ", " << position.y << ")\n";
        
        // Check if stepped on trap
        // (You could add this if traps should affect enemies)
    } else {
        // Try alternative direction
        Position altPos = position;
        if (playerPos.x > position.x) altPos.x++;
        else if (playerPos.x < position.x) altPos.x--;
        
        if (field->isValidPosition(altPos) && !field->hasBuildingAt(altPos)) {
            position = altPos;
            std::cout << "Enemy moves to (" << position.x << ", " << position.y << ")\n";
        }
    }
}

void MovingEnemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        alive = false;
        std::cout << "Enemy defeated!\n";
        // Could give points to player here
    }
}

bool MovingEnemy::isAlive() const {
    return alive && health > 0;
}

Position MovingEnemy::getPosition() const {
    return position;
}

void MovingEnemy::setPosition(const Position& pos) {
    position = pos;
}