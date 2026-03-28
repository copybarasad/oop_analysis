#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>
#include "playerhand.h"

class Enemy;
class GameField;

class Player
{
private:
  std::string name;
  int health;
  int maxHealth;
  int damage;
  int score;
  int posX;
  int posY;
  PlayerHand hand;

public:
  Player(const std::string &playerName, int initialHealth, int playerDamage, int handSize = 5);

  int getHealth() const;
  int getMaxHealth() const;
  int getDamage() const;
  int getScore() const;
  std::string getName() const;
  int getPosX() const;
  int getPosY() const;
  bool isAlive() const;
  PlayerHand &getHand();
  const PlayerHand &getHand() const;

  void setHealth(int newHealth);
  void setMaxHealth(int newMaxHealth);
  void setDamage(int newDamage);
  void setScore(int newScore);
  void setPosition(int x, int y);

  void takeDamage(int damageAmount);
  void addScore(int points);
  void heal(int amount);
  void attack(Enemy &enemy) const;
  void move(int newX, int newY);

  void castSpell(int spellIndex, GameField &field, int targetX, int targetY);
  void enemyDefeated();
};

#endif