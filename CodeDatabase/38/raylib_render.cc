#include "raylib_render.h"
#include "cell.h"
#include "defs.h"
#include "enemies.h"
#include "enemy.h"
#include "enemytower.h"
#include "hand.h"
#include "player.h"
#include "trap.h"
#include <raylib.h>
void RaylibRender::render(Player *player, Field *field, Hand &hand,
                          Enemies &enemies, std::vector<Trap> &traps,
                          std::vector<EnemyTower> &towers,
                          std::vector<std::pair<int, int>> squareHighlight) {

  DrawText(TextFormat("Live: %d", player->getLives()), 820, 50, 48, RED);
  DrawText(TextFormat("Spells:"), 820, 150, 48, BLUE);
  DrawText(persistent_str.c_str(), 820, 200 + 50 * 4, 26, RED);

  for (int x = 0; x < 20; x++) {
    for (int y = 0; y < 20; y++) {
      // DrawRectangle(x*cellwidth, y*cellheight,cellwidth,cellheight,x%2 == 0
      // ? BLACK:WHITE); DrawText(TextFormat("%d, %d",y,x),x*cellwidth,
      // y*cellheight,24,WHITE);
      // Gridlines

      DrawRectangle((x * cellwidth) + cellwidth, y * cellheight, 1, cellheight,
                    WHITE);
      DrawRectangle(x * cellwidth, y * cellheight + cellheight, cellwidth, 1,
                    WHITE);

      hand.DrawHand();

      // MAIN DRAW CALLS
      Cell &c = field->getCell(x, y);

      if (c.getEntity() == Cell::PLAYER) {
        player->drawPlayer(cellwidth, cellheight);

      } else if (c.getEntity() == Cell::ENEMY) {
        for (Enemy &enemy : enemies.enemies) {
          enemy.DrawEnemy(cellwidth, cellheight, x, y);
        }
      }
    }
  }
  for (auto &[hx, hy] : squareHighlight) {
    DrawRectangleLines(hx * cellwidth, hy * cellheight, cellwidth, cellheight,
                       RED);
    DrawRectangle(hx * cellwidth, hy * cellheight, cellwidth, cellheight,
                  Fade(RED, 0.3f));
  }
  for (auto &trap : traps) {
    int cellW = SCREENW / 20;

    int cellH = SCREENH / 20;
    DrawRectangle(trap.getX() * cellW, trap.getY() * cellH, cellW, cellH,
                  Fade(YELLOW, 0.5f));
    DrawText("X", trap.getX() * cellW + cellW / 4,
             trap.getY() * cellH + cellH / 4, 24, RED);
  }
  for (auto &tower : towers) {
    int cellW = SCREENW / 20;
    int cellH = SCREENH / 20;
    DrawRectangle(tower.getX() * cellW, tower.getY() * cellH, cellW, cellH,
                  Fade(MAROON, 0.5f));
    DrawText("T", tower.getX() * cellW + cellW / 4,
             tower.getY() * cellH + cellH / 4, 24, BLACK);
    DrawCircleLines(tower.getX() * cellW + cellW / 2,
                    tower.getY() * cellH + cellH / 2, 1 * cellW,
                    Fade(RED, 0.3f));
  }
}
