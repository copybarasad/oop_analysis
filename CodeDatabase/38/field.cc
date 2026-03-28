#include "field.h"

Field::Field(unsigned int width, unsigned int height, Player &player)
    : width(width), height(height) {
  if (width == 0 && height == 0)
    std::invalid_argument("Width and height have to be more than 0");

  for (int y = 0; y < height; y++) {
    std::vector<Cell> tmp_cells;
    for (int x = 0; x < width; x++) {
      if (x == player.getX() && y == player.getY()) {
        tmp_cells.emplace_back(Cell::PLAYER);
      } else
        tmp_cells.emplace_back(Cell::EMPTY);
    }
    field.push_back(tmp_cells);
  }
}

void Field::displayField() const {
  for (std::vector<Cell> cell : field) {
    for (Cell c : cell) {
      Cell::State entity;
      if ((entity = c.getEntity()) == Cell::EMPTY) {
        std::cout << ". ";
      } else if (entity == Cell::PLAYER) {
        std::cout << "\033[36mO\033[0m ";
      } else {
        std::cout << "\033[31mX\033[0m";
      }
    }
    puts("");
  }
}

void Field::updateField(Player &player, std::vector<Enemy> &enemies) {
  for (auto &row : field) {
    for (Cell &cell : row) {
      cell.setState(Cell::EMPTY);
      cell.setID(Cell::EMPTY);
    }
  }

  if (player.getX() < width && player.getY() < height) {
    Cell &playerCell = field[player.getY()][player.getX()];
    playerCell.setState(Cell::PLAYER);
    playerCell.setID(Cell::PLAYER);
  }

  for (Enemy &enemy : enemies) {
    // std::cout << enemy.getX() << ", " << enemy.getY() << std::endl;
    if (enemy.getX() < width && enemy.getY() < height) {
      Cell &enemyCell = field[enemy.getY()][enemy.getX()];
      enemyCell.setState(Cell::ENEMY);
      enemyCell.setID(enemy.getID());
    }
  }
}

Cell &Field::getCell(unsigned int x, unsigned int y) {
  if (x >= width || y >= height)
    throw std::out_of_range("x and/or y are out of bounds");
  return field[y][x];
}

bool Field::isValidCoord(int x, int y) {
  if (x >= width || y >= height || x < 0 || y < 0) {
    return false;
  }
  return true;
}

bool Field::isValidY(int y) { return y <= 0 || y >= height ? false : true; }

bool Field::isValidX(int x) { return x <= 0 || x >= width ? false : true; }

bool Field::isCellEmpty(int x, int y) {
  if (!isValidCoord(x, y))
    return false;
  return field[y][x].isEmpty();
}
