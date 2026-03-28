#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "cell.hpp"
#include <vector>
#include <iostream>

class Enemy;

class Player {
  private:
    int hp;
    int damage;
    int score;
    int x, y;
    int melee_damage;
    int ranged_damage;
    bool is_ranged;
  
  public:
    Player(int hp, int damage);
    bool getIsRangedCombat() const;
    int get_score() const;
    int get_x() const;
    int get_y() const;
    int get_damage() const;
    int get_hp() const;
    void switch_style();
    void take_damage(int damag);
    bool is_alive() const;
    void add_score(int points);
    void move(int dx, int dy, const std::vector<std::vector<Cell>> &, std::vector<Enemy>&);
    void rangedAttack(int dx, int dy, std::vector<Enemy>& enemies);
    bool spendScore(int amount);
    
    // Добавленные сеттеры
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void setHP(int newHP) { hp = newHP; }
    void setScore(int newScore) { score = newScore; }
    void setCombatStyle(bool ranged) { 
      is_ranged = ranged; 
      damage = ranged ? ranged_damage : melee_damage;
    }
  };

#endif