#pragma once
#include "unit.h"
#include "point.h"

class Enemy : public Unit {
protected:
  void on_move_to_point(Point& point) override;

public:
    Enemy(Grid& grid, int x, int y);
};