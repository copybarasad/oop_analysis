#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class Player;

class Enemy
{
private:
  std::string type;
  int health;
  int maxHealth;
  int damage;
  int posX;
  int posY;
  int id;
  static int nextId;

public:
  Enemy(const std::string &enemyType, int initialHealth, int enemyDamage);

  int getHealth() const;
  int getMaxHealth() const;
  int getDamage() const;
  std::string getType() const;
  int getPosX() const;
  int getPosY() const;
  int getId() const;
  bool isAlive() const;

  void setHealth(int newHealth);
  void setMaxHealth(int newMaxHealth);
  void setDamage(int newDamage);
  void setPosition(int x, int y);
  void setId(int newId);

  void takeDamage(int damageAmount);
  void heal(int amount);
  void attack(Player &player) const;
  void move(int newX, int newY);
};

#endif 