#include "Building.h"

Building::Building(int x, int y, int health, bool IsNotDestroyed)
    : position{x, y}, health(health), NotDestroyed(IsNotDestroyed) {}

void Building::takeDamage(int damage){
    health -= damage;
    if (health <= 0) {
        health = 0;
        NotDestroyed = false;
    }
}

bool Building::IsNotDestroyed(){
    return NotDestroyed;
}


int Building::getX() const { return position.x; }
int Building::getY() const { return position.y; }
int Building::getHealth() const { return health; }
bool Building::getIsNotDestroyed() const { return NotDestroyed; }

void Building::setPosition(int x, int y) {
    position.x = x; 
    position.y = y;
}
void Building::setHealth(int newHealth) { health = newHealth; }
void Building::setIsNotDestroyed(bool status) { NotDestroyed = status; }
