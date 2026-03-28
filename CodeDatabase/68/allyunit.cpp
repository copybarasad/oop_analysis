#include "allyunit.h"
#include "gamefield.h"
#include "player.h"
#include <iostream>

AllyUnit::AllyUnit(Position pos, int health, int damage) 
    : position(pos), health(health), damage(damage), alive(true) {}

void AllyUnit::takeTurn(Player* player, GameField* field) {
    if (!alive || !field) return;
    
    // Simple ally: move toward nearest enemy or protect player
    moveTowardEnemy(field);
    std::cout << "Ally at (" << position.x << ", " << position.y << ") moves\n";
}

void AllyUnit::moveTowardEnemy(GameField* field) {
    // Simple movement logic
    Position newPos = position;
    
    // Try to move right, then down
    newPos.x += 1;
    if (!field->isValidPosition(newPos) || field->hasBuildingAt(newPos)) {
        newPos = position;
        newPos.y += 1;
    }
    
    if (field->isValidPosition(newPos) && !field->hasBuildingAt(newPos)) {
        position = newPos;
    }
}

void AllyUnit::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        alive = false;
        std::cout << "Ally defeated!\n";
    }
}

bool AllyUnit::isAlive() const {
    return alive && health > 0;
}

Position AllyUnit::getPosition() const {
    return position;
}