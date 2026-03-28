#include "cell.h"

Cell::Cell(): state(Empty) {}
Cell::Cell(State initialState): state(initialState) {}
Cell::State Cell::getState() const {
  return state;
}

void Cell::setState(State newState) {
  state = newState;
}

std::string Cell::toString() const {
  switch (state) {
  case Empty: return ".";
  case Player: return "P";
  case Enemy: return "E";
  case Wall: return "#";
  case EnemyBuilding: return "B";
  case SlowCell: return "*";
  case Trap: return "T";
  case EnemyTower: return "W";
  case Ally: return "A";
  default: return "?";
  }
}