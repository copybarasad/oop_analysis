#include "enemy.hpp"
#include "player.hpp"
#include <vector>

Enemy::Enemy(int startX, int startY)
    : hp(25), damage(10), pos_x(startX), pos_y(startY) {}

bool Enemy::is_alive() const { return hp > 0; }

int Enemy::get_damage() const { return damage; }

int Enemy::get_hp() const { return hp; }

int Enemy::get_x() const { return pos_x; }

int Enemy::get_y() const { return pos_y; }

void Enemy::take_damage(int damage) {
  hp -= damage;
  if (hp <= 0) {
    hp = 0;
  }
}

void Enemy::move(int dx, int dy, const std::vector<std::vector<Cell>> &field,
                 Player &player, const std::vector<Enemy>& allEnemies) {  
  int newX = pos_x + dx;
  int newY = pos_y + dy;
  if (newX == player.get_x() && newY == player.get_y()) {
    player.take_damage(damage);
    return;
  }
  for (const auto& otherEnemy : allEnemies) {
    if (&otherEnemy != this && otherEnemy.is_alive() && 
        otherEnemy.get_x() == newX && otherEnemy.get_y() == newY) {
      return;  
    }
  }
  if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size() &&
      field[newX][newY].get_isPass()) {
    pos_x = newX;
    pos_y = newY;
  }
}