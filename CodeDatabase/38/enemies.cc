#include "enemies.h"
#include "field.h"
#include "player.h"
#include <algorithm>
#include <sstream>
void Enemies::updateEnemies(Field &field, Player &player,
                            std::vector<Trap> &traps, Logger &l) {
  enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                               [](const Enemy &e) { return !e.isAlive(); }),
                enemies.end());
  int targetX = player.getX();
  int targetY = player.getY();
  std::vector<std::pair<int, int>> newPositions;
  newPositions.reserve(enemies.size());

  for (auto &enemy : enemies) {

    int enemyX = enemy.getX();
    int enemyY = enemy.getY();
    int *newPos = enemy.enemyMover(targetX, targetY);
    int newX = newPos[0];
    int newY = newPos[1];
    delete[] newPos;
    // Prevent overlapping enemies
    bool occupied = false;
    for (auto &other : newPositions) {
      if (other.first == newX && other.second == newY)
        occupied = true;
    }

    if (!field.isValidCoord(newX, newY) || !field.isCellEmpty(newX, newY) ||
        occupied) {
      newX = enemyX;
      newY = enemyY;

      l.logEvent(Events::ENEMY_KILLED, NULL);
    }

    newPositions.emplace_back(newX, newY);

    for (Trap &trap : traps) {
      if (trap.isAlive() && enemy.getX() == trap.getX() &&
          enemy.getY() == trap.getY()) {
        enemy.takeDamage(trap.getDamage());
        trap.takeDamage(1);
      }
    }
    // TAKE DAMAGE PLAYER MUHAHAHAHAHA
    for (auto &enemy : enemies) {
      // Check adjacency to player
      if (std::abs((int)(enemy.getX() - targetX)) <= 1 &&
          std::abs((int)(enemy.getY() - targetY)) <= 1) {
        player.takeDamage(enemy.getDamage());
      }
    }
  }

  // Apply new positions
  for (size_t i = 0; i < enemies.size(); ++i) {
    enemies[i].setPos(newPositions[i].first, newPositions[i].second);
  }
}
void Enemies::addEnemy(
    Field &field, std::mt19937 &mt,
    std::uniform_int_distribution<std::mt19937::result_type> &distX,
    std::uniform_int_distribution<std::mt19937::result_type> &distY,
    std::uniform_int_distribution<std::mt19937::result_type>
        &distEnemySpawner) {

  int etype = distEnemySpawner(mt);
  // std::cout << type << std::endl;
  Enemy::EnemyType e_type = static_cast<Enemy::EnemyType>(etype);

  unsigned int x = distX(mt);
  unsigned int y = distY(mt);
  while (!field.isCellEmpty(x, y)) {
    x = distX(mt);
    y = distY(mt);
  }

  enemies.emplace_back(x, y, e_type);
}
