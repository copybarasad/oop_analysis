#pragma once
#include "irender.h"
#include <vector>
class Player;
class Hand;
class Cell;
class Enemies;
class Trap;
class EnemyTower;
class Field;
class RaylibRender : public IRender {
public:
  RaylibRender(unsigned int width, unsigned int height, unsigned int cellwidth,
               unsigned int cellheight) {
    this->width = width;
    this->height = height;
    this->cellwidth = cellwidth;
    this->cellheight = cellheight;
  }
  virtual void
  render(Player *player, Field *field, Hand &hand, Enemies &enemies,
         std::vector<Trap> &traps, std::vector<EnemyTower> &towers,
         std::vector<std::pair<int, int>> squareHighlight) override;
};
