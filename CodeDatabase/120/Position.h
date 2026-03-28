#pragma once

struct Position {
  int x;
  int y;
  Position(int xValue = 0, int yValue = 0);
};

bool operator==(const Position& a, const Position& b);
bool operator!=(const Position& a, const Position& b);
