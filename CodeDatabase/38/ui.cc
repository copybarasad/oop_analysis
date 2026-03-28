#include "ui.h"
#include "player.h"
#include <raylib.h>
void drawUI(int width, int height, int cellwidth, int cellheight,
            std::string &persistent_str, Player &player) {

  DrawText(TextFormat("Live: %d", player.getLives()), 820, 50, 48, RED);
  DrawText(TextFormat("Spells:"), 820, 150, 48, BLUE);
  DrawText(persistent_str.c_str(), 820, 200 + 50 * 4, 26, RED);
}
