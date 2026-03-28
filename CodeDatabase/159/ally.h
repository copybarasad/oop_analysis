#ifndef ALLY_H
#define ALLY_H

#include <utility>

class Ally {
public:
  Ally(int startX, int startY, int initialHealth, int initialDamage);
  int getX() const;
  int getY() const;
  int getHealth() const;
  int getDamage() const;

  void move(int newX, int newY);
  void takeDamage(int damage);
  void setSlowed(bool slowed);
  bool isAlive() const;
  bool isSlowed() const;

private:
  int x, y;
  int health;
  int damage;
  bool slowed;
};

#endif