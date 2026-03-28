#ifndef ENEMY_H
#define ENEMY_H

#include "ally.h"
#include <utility>
#include <vector>

class Field;

class Enemy {
public:
  Enemy(int startX, int startY, int initialHealth, int damage);

  int getX() const;
  int getY() const;
  int getHealth() const;
  int getDamage() const;

  void move(int newX, int newY);
  void takeDamage(int damage);
  bool isAlive() const;
  
  std::pair<int, int> decideMove(int width, int height, const Field& field, int px, int py, const std::vector<Ally>& allies);

private:
  int x, y;
  int health;
  int damage;
};

#endif