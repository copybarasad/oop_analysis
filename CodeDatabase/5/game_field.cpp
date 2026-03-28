#include "game_field.hpp"
#include <random>

GameField::GameField(int width, int height) {
  if (width < 10)
    width = 10;
  if (width > 25)
    width = 25;
  if (height < 10)
    height = 10;
  if (height > 25)
    height = 25;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  field.resize(width, std::vector<Cell>(height));

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      bool isPassable = dis(gen) > 0.2;
      field[i][j] = Cell(isPassable);
    }
  }

  field[0][0].setIsPassable(true);
  field[width - 1][height - 1].setIsPassable(true);
}

GameField::GameField(const GameField &other) { field = other.field; }

GameField::GameField(GameField &&other) noexcept {
  field = std::move(other.field);
}

GameField &GameField::operator=(const GameField &other) {
  if (this != &other) {
    field = other.field;
  }
  return *this;
}

GameField &GameField::operator=(GameField &&other) noexcept {
  if (this != &other) {
    field = std::move(other.field);
  }
  return *this;
}

const std::vector<std::vector<Cell>> &GameField::getField() const {
  return field;
}

int GameField::getWidth() const { return field.size(); }

int GameField::getHeight() const { return field[0].size(); }

void GameField::display(const Player &player,
                        const std::vector<Enemy> &enemies) const {
  for (int y = 0; y < getHeight(); ++y) {
    for (int x = 0; x < getWidth(); ++x) {
      if (x == player.get_x() && y == player.get_y()) {
        std::cout << "P ";
      } else {
        bool enemyFound = false;
        for (const auto &enemy : enemies) {
          if (enemy.get_x() == x && enemy.get_y() == y && enemy.is_alive()) {
            std::cout << "E ";
            enemyFound = true;
            break;
          }
        }
        if (!enemyFound) {
          std::cout << (field[x][y].get_isPass() ? ". " : "# ");
        }
      }
    }
    std::cout << std::endl;
  }
}