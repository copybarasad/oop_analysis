#ifndef FIELD_H
#define FIELD_H

#include "cell.h"
#include <vector>
#include <utility>

class Field {
public:
  Field(int width, int height);
  Field(const Field& other);
  Field(Field&& other) noexcept;
  Field& operator=(const Field& other);
  Field& operator=(Field&& other) noexcept;
  ~Field();

  int getWidth() const;
  int getHeight() const;
  Cell& getCell(int x, int y);
  const Cell& getCell(int x, int y) const;
  std::vector<std::vector<Cell>> getGrid() const;

  void setCell(int x, int y, Cell::State state);
  void display() const;
  void resetGrid(const std::vector<std::vector<Cell>>& initial);

  bool isWall(int x, int y) const;
 
private:
  int width;
  int height;
  std::vector<std::vector<Cell>> grid;
  std::vector<std::vector<Cell>> initialGrid;

  void checkInvariant(int w, int h) const;
};

#endif 