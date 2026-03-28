#ifndef RPG_ENTITY_H_
#define RPG_ENTITY_H_

#include "Position.h"
#include <cmath>

class Entity {
private:
  int damage;
  int health;

protected:
  Position position;

public:
  Entity(int damage, int health, Position position = Position(0, 0))
      : damage(damage), health(health), position(position) {}
  void takeDamage(int taken_damage) {
    health -= taken_damage;
    if (health < 0)
      health = 0;
  }
  Position getPosition() const { return position; }
  void setPosition(const Position &newPos) { position.set(newPos); }

  bool isAlive() const { return health > 0; }
  int getDamage() const { return damage; }
  int getHealth() const { return health; }
  void moveUp() { position.moveUp(); }
  void moveDown() { position.moveDown(); }
  void moveLeft() { position.moveLeft(); }
  void moveRight() { position.moveRight(); }
  int distanceTo(const Entity &other) const {
    return position.distanceTo(other.getPosition());
  }
  bool isCollidingWith(const Entity &other) const {
    return position == other.getPosition();
  }
  void attack(Entity &target) const { target.takeDamage(damage); }
};

#endif // RPG_ENTITY_H_
