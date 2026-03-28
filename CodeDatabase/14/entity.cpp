#include "entity.hpp"
#include "player.hpp"      
#include "GameBoard.hpp"

Entity::Entity(int x, int y) : x_coordinate(x), y_coordinate(y) {}
int Entity::getX() const { return x_coordinate; }
int Entity::getY() const { return y_coordinate; }
void Entity::setPosition(int x, int y) { x_coordinate = x; y_coordinate = y; }

void Entity::takeDamage(int dmg) { (void)dmg; }
void Entity::onDeath(Player* player) { (void)player; }
bool Entity::isEnemy() const { return false; }
bool Entity::isTrap() const { return false; }
int Entity::getHealth() const { return -1; }
void Entity::onSteppedOn(Entity* activator) { (void)activator; }
