#pragma once
#include "defs.h"
#include "entity.h"
#include "input.h"
#include "logger.h"
#include <string>
class Field;
class Game;
class Hand;
class Player : public Entity {
private:
  Logger &logger;

public:
  typedef enum {
    NOTHING,
    MOVED,
    REJECT_MOVE,
    DIRECTSPELL,
    AREASPELL,
    TRAPSPELL,
    UNSET

  } PlayerMoves;
  Player(unsigned int x, unsigned int y, Logger &l) : logger(l) {
    this->x = x;
    this->y = y;
    this->lives = 100;
  }
  unsigned int getX() const { return x; }

  unsigned int getY() const { return y; }
  void addHealth(unsigned int amount);

  bool isTrapped(Field &f, Hand &hand);
  void drawGameOverDead();
  void drawGameOverTrapped();
  PlayerMoves move_player(Field &field, Game &game, COMMAND cmd);
  void drawPlayer(int cellwidth, int cellheight);
};
