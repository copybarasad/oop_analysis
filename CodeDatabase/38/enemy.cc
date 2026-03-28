#include "enemy.h"
#include "game.h"
#include "raylib.h"
#include <algorithm>
#include <sstream>
void Enemy::DrawEnemy(int cellwidth, int cellheight, int x, int y) {
  if (getX() == x && getY() == y) {
    switch (getType()) {
    case Enemy::EnemyType::DIRECT:
      DrawCircle(x * cellwidth + cellwidth / 2, y * cellheight + cellheight / 2,
                 (float)cellwidth / 2 - 3, RED);
      break;

    case Enemy::EnemyType::PREDICT:
      DrawCircle(x * cellwidth + cellwidth / 2, y * cellheight + cellheight / 2,
                 (float)cellwidth / 2 - 3, PURPLE);
      break;

    case Enemy::EnemyType::AMBUSH:
      DrawCircle(x * cellwidth + cellwidth / 2, y * cellheight + cellheight / 2,
                 (float)cellwidth / 2 - 3, GREEN);
      break;

    case Enemy::EnemyType::WANDER:
      DrawCircle(x * cellwidth + cellwidth / 2, y * cellheight + cellheight / 2,
                 (float)cellwidth / 2 - 3, GRAY);
      break;
    }
  }
}

int *Enemy::enemyMover(int targetX, int targetY) {
  int dx = 0, dy = 0;

  switch (type) {
  case DIRECT:
    dx = (targetX > x) ? 1 : (targetX < x ? -1 : 0);
    dy = (targetY > y) ? 1 : (targetY < y ? -1 : 0);
    break;

  case PREDICT: {
    int predictX = targetX + (targetX - x) / 2;
    int predictY = targetY + (targetY - y) / 2;
    predictX = std::clamp(predictX, 0, SCREENW / 20 - 1);
    predictY = std::clamp(predictY, 0, SCREENH / 20 - 1);
    dx = (predictX > x) ? 1 : (predictX < x ? -1 : 0);
    dy = (predictY > y) ? 1 : (predictY < y ? -1 : 0);
    break;
  }

  case AMBUSH: {
    // Try to move to player’s side (flanking)
    dx = (targetX > x) ? 1 : (targetX < x ? -1 : 0);
    dy = (targetY > y) ? 1 : (targetY < y ? -1 : 0);
    // offset movement pattern for flank
    if (abs((int)(targetX - x)) < abs((int)(targetY - y)))
      dx *= -1;
    break;
  }

  case WANDER: {
    // Randomly pick between chasing or wandering
    if (GetRandomValue(0, 3) == 0) {
      dx = GetRandomValue(-1, 1);
      dy = GetRandomValue(-1, 1);
    } else {
      dx = (targetX > x) ? 1 : (targetX < x ? -1 : 0);
      dy = (targetY > y) ? 1 : (targetY < y ? -1 : 0);
    }
    break;
  }
  }

  int newX = x + dx;
  int newY = y + dy;

  int *newPos = new int[2];
  *newPos = newX;
  newPos[1] = newY;

  return newPos;
}

void Enemy::moveEnemy(unsigned int x, unsigned int y, Logger &l) {
  this->x = x;
  this->y = y;
}
