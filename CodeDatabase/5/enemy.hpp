#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "cell.hpp"
#include <vector>

class Player;

class Enemy {
private:
  int hp;
  int damage;
  int pos_x, pos_y;

public:
  Enemy(int startX, int startY);
  void move(int dx, int dy, const std::vector<std::vector<Cell>> &field, 
  Player &player, const std::vector<Enemy>& allEnemies);
  void take_damage(int damage);
  bool is_alive() const;
  int get_x() const;
  int get_y() const;
  int get_damage() const;
  int get_hp() const;
  void setHP(int newHP) { hp = newHP; }
  void setPosition(int newX, int newY) { pos_x = newX; pos_y = newY; }
  void setDamage(int newDamage) { damage = newDamage; }
};

#endif