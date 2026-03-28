#include "Field.h"

void Field::fillWithImpassableCells() {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int wallPercent = Random::getInt(1, 100);
      if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
        field[y][x] = Cell(CellType::impassable);
      } else if (wallPercent <= 15) {
        field[y][x] = Cell(CellType::impassable);
      } else if (wallPercent <= 30) {
        field[y][x] = Cell(CellType::slowing);
      } else {
        field[y][x] = Cell(CellType::empty);
      }
    }
  }
}

Field::Field(int width, int height) {
  if (validDimensions(width, height)) {
    this->width = width;
    this->height = height;
  } else {
    this->width = 15;
    this->height = 15;
  }
  this->field =
      std::vector<std::vector<Cell>>(height, std::vector<Cell>(width));
  fillWithImpassableCells();
}

Field::Field(const Field &other)
    : width(other.width), height(other.height), field(other.field) {
  field = std::vector<std::vector<Cell>>(height, std::vector<Cell>(width));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      field[i][j] = other.field[i][j];
    }
  }
}

Field::Field(Field &&other) noexcept
    : width(other.width), height(other.height), field(std::move(other.field)) {
  other.width = 0;
  other.height = 0;
}

Field &Field::operator=(const Field &other) {
  if (this != &other) {
    width = other.width;
    height = other.height;
    field = other.field;
  }
  return *this;
}

Field &Field::operator=(Field &&other) noexcept {
  if (this != &other) {
    width = other.width;
    height = other.height;
    field = std::move(other.field);
  }
  other.width = 0;
  other.height = 0;
  other.field = std::vector<std::vector<Cell>>();
  return *this;
}

Cell &Field::getCell(const Position &pos) {
  if (!isValidPosition(pos)) {
    throw std::out_of_range("Cell coordinates out of bounds");
  }
  return field[pos.getY()][pos.getX()];
}

const Cell &Field::getCell(const Position &pos) const {
  if (!isValidPosition(pos)) {
    throw std::out_of_range("Cell coordinates out of bounds");
  }
  return field[pos.getY()][pos.getX()];
}

bool Field::isValidPosition(const Position &pos) const {
  return pos.getX() >= 0 && pos.getX() < width && pos.getY() >= 0 &&
         pos.getY() < height;
}

bool Field::canMoveTo(const Position &pos) const {
  return isValidPosition(pos) && getCell(pos).isPassable();
}