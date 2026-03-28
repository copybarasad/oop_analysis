#ifndef CELL_H
#define CELL_H
#include <string>

class Cell {
public:
  enum State {Empty, Player, Enemy, Wall, EnemyBuilding, SlowCell, Trap, EnemyTower, Ally};
  Cell();
  Cell(State initialState);
  State getState() const;
  void setState(State newState);

  std::string toString() const;

private:
  State state;
};

#endif