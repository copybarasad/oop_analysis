#pragma once
#include <ctime>
#include <optional>
#include <random>

#include "Cell.h"
#include "Position.h"

class GameMap {
 private:
  int width;
  int height;
  std::vector<std::vector<Cell>> cells;

 public:
  GameMap(int w, int h);

  GameMap(const GameMap& other);
  GameMap(GameMap&& other);
  GameMap& operator=(const GameMap& other);
  GameMap& operator=(GameMap&& other);

  int GetWidth() const;
  int GetHeight() const;
  Cell::CellType GetCellType(int x, int y) const;
  std::vector<Position> FindFreeAdjacent(const Position& pos) const;
  bool IsInside(int x, int y) const;
  bool IsWalkable(int x, int y) const;
  std::optional<Position> FindRandomFreeCell() const;
  void SetCells(std::vector<std::vector<Cell>>&& newCells);
};
