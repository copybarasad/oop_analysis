#ifndef RPG_FIELD_H_
#define RPG_FIELD_H_

#include "Cell.h"
#include "Position.h"
#include "Random.h"
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>

class Field {
private:
  int width;
  int height;
  std::vector<std::vector<Cell>> field;
  bool validDimensions(int width, int height) {
    return width >= 10 && width <= 25 && height >= 10 && height <= 25;
  }
  void fillWithImpassableCells();

public:
  Field() : width(10), height(10), field(10, std::vector<Cell>(10)) {
    fillWithImpassableCells();
  }

  Field(int width, int height);
  Field(const Field &other);
  Field(Field &&other) noexcept;
  Field &operator=(const Field &other);
  Field &operator=(Field &&other) noexcept;
  ~Field() = default;

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  Cell &getCell(const Position &pos);
  const Cell &getCell(const Position &pos) const;
  bool isValidPosition(const Position &pos) const;
  bool canMoveTo(const Position &pos) const;
};

#endif // RPG_FIELD_H_
