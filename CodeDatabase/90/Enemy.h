#ifndef RPG_ENEMY_H_
#define RPG_ENEMY_H_

#include "Entity.h"
#include "Player.h"
#include "Position.h"
#include <random>

class Enemy : public Entity {
private:
  static std::mt19937 rng;
  bool slowed;
  bool adjacentToPlayer;

public:
  Enemy(int damage, int health, Position position = Position(0, 0))
      : Entity(damage, health, position), slowed(false),
        adjacentToPlayer(false) {}
  void move(const Player &player, int fieldWidth, int fieldHeight);
  void randomMove(int fieldWidth, int fieldHeight);
  bool isSlowed() const { return slowed; }
  void setSlowed(bool value) { slowed = value; }
  bool wasAdjacentToPlayer() const { return adjacentToPlayer; }
  void setAdjacentToPlayer(bool value) { adjacentToPlayer = value; }
};

#endif // RPG_ENEMY_H_
