#pragma once
#include "entity.h"
#include "logger.h"
#include <climits>
#include <random>
class Enemy : public Entity {

public:
  typedef enum { DIRECT = 0, PREDICT, AMBUSH, WANDER } EnemyType;
  Enemy(unsigned int x, unsigned int y, EnemyType type = DIRECT)
      : type(type), damage(2) {
    this->x = x;
    this->y = y;
    this->lives = 60;
    std::random_device dev;
    std::mt19937 mt(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(5,
                                                                  INT_MAX - 1);
    ID = dist(mt);
  }
  int getID() const { return ID; }

  void moveEnemy(unsigned int x, unsigned int y, Logger &l);
  void DrawEnemy(int cellwidth, int cellheight, int x, int y);
  int getType() const { return type; }
  int getDamage() const { return damage; }
  int *enemyMover(int targetX, int targetY);

private:
  int ID;
  EnemyType type;
  int damage;
};
