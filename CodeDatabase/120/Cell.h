#pragma once

class Cell {
 public:
  enum class CellType { Empty, Wall, Slow };

 private:
  CellType type;

 public:
  Cell(CellType initialType = CellType::Empty);
  CellType GetType() const;
  void SetType(CellType newType);
  bool IsWalkable() const;
};
