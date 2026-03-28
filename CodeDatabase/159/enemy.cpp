#include "enemy.h"
#include "field.h"
#include <cstdlib>
#include <ctime>
#include <utility>
#include <cmath>

Enemy::Enemy(int startX, int startY, int initialHealth, int initialDamage): x(startX), y(startY), health(initialHealth), damage(initialDamage) {}

int Enemy::getX() const {return x;}
int Enemy::getY() const {return y;}
int Enemy::getHealth() const {return health;}
int Enemy::getDamage() const {return damage;}

void Enemy::move(int newX, int newY) {
  x = newX;
  y = newY;
}

void Enemy::takeDamage(int initialDamage) {
  health -= initialDamage;
  if (health<0) health = 0;
}

bool Enemy::isAlive() const {
  return health>0;
}

std::pair<int, int> Enemy::decideMove(int width, int height, const Field& field, int px, int py, const std::vector<Ally>& allies) {
  int minDistance = std::abs(px - x) + std::abs(py - y);
  int targetX = px;
  int targetY = py;
  for (const auto& ally: allies) {
    if (ally.isAlive()) {
      int dist = std::abs(ally.getX()-x) + std::abs(ally.getY()-y);
      if (dist<minDistance) {
        minDistance = dist;
        targetX = ally.getX();
        targetY = ally.getY();
      }
    }
  }

  int distance = minDistance;
  if (distance<=3 && distance>1) {
    int newX = x;
    int newY = y;
    if (targetX>x && !field.isWall(x+1, y)) newX = x+1;
    else if(targetX<x && !field.isWall(x-1, y)) newX = x-1;
    else if(targetY>y && !field.isWall(x, y+1)) newY = y+1;
    else if(targetY<y && !field.isWall(x, y-1)) newY = y-1;
    if (newX!=x || newY!=y) {
      Cell::State state = field.getCell(newX, newY).getState();
      if (state == Cell::Empty || state == Cell::SlowCell || state == Cell::Trap) {
        return {newX, newY};
      }
    }
  }

  int direction = rand()%4;
  int newX = x;
  int newY = y;

  switch (direction) {
    case 0:
      newY--;
      break;
    case 1:
      newY++;
      break;
    case 2:
      newX--;
      break;
    case 3:
      newX++;
      break;
  }

  if (newX<0) newX = 0;
  if (newX>=width) newX = width-1;
  if (newY<0) newY = 0;
  if (newY>=height) newY = height-1;

  return {newX, newY};
}