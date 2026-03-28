#include "Enemy.h"
#include "Player.h"
#include <vector>

std::mt19937 Enemy::rng(std::random_device{}());

void Enemy::move(const Player &player, int fieldWidth, int fieldHeight) {
  if (distanceTo(player) > 3) {
    randomMove(fieldWidth, fieldHeight);
    return;
  }
  std::vector<Position> possibleMoves;
  Position self = getPosition();
  if (self.getX() > 0)
    possibleMoves.emplace_back(self.getX() - 1, self.getY());
  if (self.getX() < fieldWidth - 1)
    possibleMoves.emplace_back(self.getX() + 1, self.getY());
  if (self.getY() > 0)
    possibleMoves.emplace_back(self.getX(), self.getY() - 1);
  if (self.getY() < fieldHeight - 1)
    possibleMoves.emplace_back(self.getX(), self.getY() + 1);

  auto bestMove = possibleMoves.begin();
  int minDistance = std::numeric_limits<int>::max();

  for (auto it = possibleMoves.begin(); it != possibleMoves.end(); ++it) {
    int distance = player.getPosition().distanceTo(*it);

    if (distance < minDistance) {
      minDistance = distance;
      bestMove = it;
    }
  }

  if (bestMove != possibleMoves.end()) {
    int newX = bestMove->getX();
    int newY = bestMove->getY();
    int curX = self.getX();
    int curY = self.getY();

    if (newX != curX) {
      if (newX > curX)
        moveRight();
      else
        moveLeft();
    } else if (newY != curY) {
      if (newY > curY)
        moveDown();
      else
        moveUp();
    }
  }
}

void Enemy::randomMove(int fieldWidth, int fieldHeight) {
  std::vector<Position> possibleMoves;
  Position self = getPosition();
  if (self.getX() > 0)
    possibleMoves.emplace_back(self.getX() - 1, self.getY());
  if (self.getX() < fieldWidth - 1)
    possibleMoves.emplace_back(self.getX() + 1, self.getY());
  if (self.getY() > 0)
    possibleMoves.emplace_back(self.getX(), self.getY() - 1);
  if (self.getY() < fieldHeight - 1)
    possibleMoves.emplace_back(self.getX(), self.getY() + 1);

  if (!possibleMoves.empty()) {
    std::uniform_int_distribution<int> dist(0, possibleMoves.size() - 1);
    Position chosen = possibleMoves[dist(rng)];
    int newX = chosen.getX();
    int newY = chosen.getY();
    int curX = self.getX();
    int curY = self.getY();
    if (newX != curX) {
      if (newX > curX)
        moveRight();
      else
        moveLeft();
    } else if (newY != curY) {
      if (newY > curY)
        moveDown();
      else
        moveUp();
    }
  }
}