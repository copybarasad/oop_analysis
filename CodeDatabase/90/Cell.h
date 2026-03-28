#ifndef RPG_CELL_H_
#define RPG_CELL_H_

enum class CellType { empty, impassable, slowing };

class Cell {
private:
  CellType type;

public:
  Cell() : type(CellType::empty) {}
  Cell(CellType cellType) : type(cellType) {}
  CellType getType() const { return type; }
  void setType(CellType newType) { type = newType; }
  bool isPassable() const { return type != CellType::impassable; }
};

#endif // RPG_CELL_H_
