#pragma once
#include "player.h"
#include <string>
class UI {
public:
  UI() {}
  void drawUI(int width, int height, int cellwidth, int cellheight,
              std::string &persistent_str, Player &player);
};
