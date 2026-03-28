#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include "cell.hpp"
#include "enemy.hpp"
#include "player.hpp"
class GameField {
private:
  std::vector<std::vector<Cell>> field;

public:
  GameField(int width, int height);
  GameField(const GameField &other);
  GameField(GameField &&other) noexcept;
  GameField &operator=(const GameField &other);
  GameField &operator=(GameField &&other) noexcept;

  const std::vector<std::vector<Cell>> &getField() const;
  int getWidth() const;
  int getHeight() const;
  void display(const Player &player, const std::vector<Enemy> &enemies) const;
};

#endif