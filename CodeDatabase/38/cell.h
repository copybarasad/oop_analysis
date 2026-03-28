#pragma once

class Cell {
private:
  int ID; // EMPTY = ID -1
          // PLAYER = ID 0
          // TOWER = 1
          // ENEMY = the rest
          // DEPRECATED
public:
  typedef enum { EMPTY = -1, PLAYER, TOWER, ENEMY } State;
  Cell(State state) : state(state), ID(state) {}
  State getEntity() const { return state; }
  bool isPlayerHere() const { return (state == PLAYER); }
  bool isTowerHere() const { return (state == TOWER); }
  bool isEnemyHere() const { return (state == ENEMY); }

  bool isEmpty() const { return (state == EMPTY); }
  void setState(State state) { this->state = state; }
  int getID() const { return ID; }
  void setID(int ID) { this->ID = ID; }

private:
  State state;
};
