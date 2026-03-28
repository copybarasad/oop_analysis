#pragma once
#include "cell.h"
#include "enemy.h"
#include "player.h"
#include <iostream>
#include <vector>
class Field {
private:
  std::vector<std::vector<Cell>> field;
  unsigned int width;
  unsigned int height;

public:
  Field(unsigned int width, unsigned int height, Player &player);
  void displayField() const;

  void updateField(Player &player, std::vector<Enemy> &enemies);

  Cell &getCell(unsigned int x, unsigned int y);

  bool isValidCoord(int x, int y);
  bool isValidY(int y);
  bool isValidX(int x);
  bool isCellEmpty(int x, int y);
};
