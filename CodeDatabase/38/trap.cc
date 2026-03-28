#include "trap.h"
#include <vector>
bool placeTrap(int x, int y, Field &field, std::vector<Trap> &traps){
    if (!field.isValidCoord(x, y))
    return false;

  // Prevent placing on top of existing trap
  for (auto &t : traps)
    if (t.getX() == x && t.getY() == y && t.isActive())
      return false;

  traps.emplace_back(x, y);
  return true;
}