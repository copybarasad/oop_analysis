#ifndef CELL_HPP
#define CELL_HPP
class Cell {
private:
  bool isPass;

public:
  Cell(bool isPass = true);
  bool get_isPass() const;
  void setIsPassable(bool passable);
};

#endif