#pragma once
#include <cstdlib>
#include <vector>
class Player;
class Hand;
class Cell;
class Enemies;
class Trap;
class EnemyTower;
class Field;
template <typename Drawer> class Visualizer {
private:
  Drawer drawer;

public:
  Visualizer(uint width, uint height, uint cellwidth, uint cellheight)
      : drawer(width, height, cellwidth, cellheight) {}
  void render_call(Player *player, Field *field, Hand &hand, Enemies &enemies,
                   std::vector<Trap> &traps, std::vector<EnemyTower> &towers,
                   std::vector<std::pair<int, int>> squareHighlight) {
    drawer.render(player, field, hand, enemies, traps, towers, squareHighlight);
  }
};
