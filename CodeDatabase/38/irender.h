#pragma once
#include <vector>
class Player;
class Field;
class Hand;
class Enemies;
class Trap;
class EnemyTower;
class IRender {
protected:
  // width and height here arent actual width and height
  // but how many cells span across
  unsigned int width;
  unsigned int height;
  unsigned int cellwidth;
  unsigned int cellheight;

  // virtual void drawPlayer();
  // virtual void drawEnemies();
  // virtual void drawTraps();
  // virtual void drawTowers();

public:
  virtual void render(Player *player, Field *field, Hand &hand,
                      Enemies &enemies, std::vector<Trap> &traps,
                      std::vector<EnemyTower> &towers,
                      std::vector<std::pair<int, int>> squareHighlight) = 0;
  IRender() {}
  virtual ~IRender() {}
};
