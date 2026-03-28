#include "field.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Field::Field(int w, int h): width(w), height(h) {
  checkInvariant(w, h);
  grid.assign(height, std::vector<Cell>(width, Cell()));
  initialGrid = grid;

  for (int y=0; y<height; ++y) {
    for (int x=0; x<width; ++x) {
      if ((x != 0 || y != 0) && rand()%5 == 0) {
        initialGrid[y][x].setState(Cell::Wall);
      }

      if ((x!=0 || y!=0) && rand()%10 == 0 && initialGrid[y][x].getState() == Cell::Empty) {
        initialGrid[y][x].setState(Cell::SlowCell);
      }
    }
  }
  grid = initialGrid;
}

Field::Field(const Field& other):
  width(other.width), height(other.height), grid(other.grid) {}

Field::Field(Field&& other) noexcept:
  width(other.width), height(other.height),
  grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
  }

Field& Field::operator=(const Field& other) {
  if (this != &other) {
    width = other.width;
    height = other.height;
    grid = other.grid;
  }
  return *this;
}

Field& Field::operator=(Field&& other) noexcept {
  if (this != &other) {
    width = other.width;
    height = other.height;
    grid = std::move(other.grid);
    other.width = 0;
    other.height = 0;
  }
  return *this;
}

Field::~Field() {}

int Field::getWidth() const {
  return width;
}

int Field::getHeight() const {
  return height;
}

Cell& Field::getCell(int x, int y) {
  return grid[y][x];
}

const Cell& Field::getCell(int x, int y) const {
  return grid[y][x];
}

std::vector<std::vector<Cell>> Field::getGrid() const {
  return grid;
}

void Field::setCell(int x, int y, Cell::State state) {
  grid[y][x].setState(state);
}

void Field::checkInvariant(int w, int h) const {
  if (w<10 || w>25 || h<10 || h>25) {
    throw std::invalid_argument("Field dimensions must be between 10 and 25.");
  }
}

void Field::display() const {
  for (int y=0; y<height; ++y) {
    for (int x=0; x<width; ++x) {
      std::cout << getCell(x, y).toString() << " ";
    }
    std::cout << std::endl;
  }
}

void Field::resetGrid(const std::vector<std::vector<Cell>>& initial) {
  grid = initial;
}

bool Field::isWall(int x, int y) const {
  if (x<0 || x>=width || y<0 || y>=height) {
    return true;
  }
  return getCell(x, y).getState() == Cell::Wall;
}