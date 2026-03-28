#include "player.h"
#include "field.h"
#include "game.h"
#include "input.h"
#include <raylib.h>
#include <sstream>
#include <unistd.h>
Player::PlayerMoves Player::move_player(Field &field, Game &game, COMMAND cmd) {
  int dxfar = 0, dx = 0;
  int dyfar = 0, dy = 0;
  // KeyboardKey key = convert.readKey();
  PlayerMoves ret = PlayerMoves::UNSET;

  switch (cmd) {
  case COMMAND::MOVED_UP:
    dy = -1, dyfar = -2;
    break;

  case COMMAND::MOVED_DOWN:
    dy = 1, dyfar = 2;
    break;

  case COMMAND::MOVED_LEFT:
    dx = -1, dxfar = -2;
    break;

  case COMMAND::MOVED_RIGHT:
    dx = 1, dxfar = 2;
    break;

  case COMMAND::DIRECTSPELL:
    ret = PlayerMoves::DIRECTSPELL;
    break;

  case COMMAND::AREASPELL:
    ret = PlayerMoves::AREASPELL;
    break;

  case COMMAND::TRAPSPELL:
    ret = PlayerMoves::TRAPSPELL;
    break;

  case COMMAND::NOTHING:
    ret = PlayerMoves::NOTHING;
    break;

  default:
    ret = PlayerMoves::UNSET;
    break;
  }

  // If no movement and no spell pressed
  if (dx == 0 && dy == 0 && ret == PlayerMoves::UNSET)
    ret = PlayerMoves::NOTHING;

  // Only move player if dx or dy
  if (dx != 0 || dy != 0) { // if dx is set dy is set (hopefully)
    int newX = x + dxfar;
    int newY = y + dyfar;
    int newAltX = x + dx;
    int newAltY = y + dy;

    if (field.isValidCoord(newX, newY) && field.isCellEmpty(newX, newY)) {
      setPos(newX, newY);
      ret = PlayerMoves::MOVED;
      game.removeHighlightArea();
    } else if (field.isValidCoord(newAltX, newAltY) &&
               field.isCellEmpty(newAltX, newAltY)) {
      setPos(newAltX, newAltY);
      ret = PlayerMoves::MOVED;
      game.removeHighlightArea();
    } else if (ret == PlayerMoves::UNSET) {
      // ret = PlayerMoves::NOTHING; // blocked move
      ret = PlayerMoves::REJECT_MOVE;
    }
  }

  return ret;
}

bool Player::isTrapped(Field &field, Hand &hand) {
  bool trapped = true, hasSpells = false;
  for (int dx = -2; dx <= 2; dx++) {
    for (int dy = -2; dy <= 2; dy++) {
      if (dx == 0 && dy == 0)
        continue;
      int nx = x + dx;
      int ny = y + dy;
      if (field.isValidCoord(nx, ny) && field.isCellEmpty(nx, ny)) {
        trapped = false;
        break;
      }
    }
    if (!trapped)
      break;
  }

  if (trapped) {

    for (auto &p : hand.showHand()) {
      if (p.second > 0) {
        hasSpells = true;
        break;
      }
    }
  }

  return trapped && hasSpells;
}

void Player::drawGameOverDead() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("YOU DIED!", 280, 350, 128, RED);
  EndDrawing();
  sleep(5);
  CloseWindow();
}
void Player::drawGameOverTrapped() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("You are trapped and out of spells! GAME OVER!", 200, 350, 48, RED);
  EndDrawing();
  sleep(5);
  CloseWindow();
}
void Player::drawPlayer(int cellwidth, int cellheight) {

  float px = x * cellwidth + (float)cellwidth / 2;
  float py = y * cellheight + (float)cellheight / 2;
  float radius = (float)cellwidth / 2;

  DrawCircle(px, py, radius + 8, Fade(SKYBLUE, 0.25f)); // big, translucent glow

  DrawCircle(px, py, radius, SKYBLUE);
  DrawCircleLines(px, py, radius, WHITE);
}

void Player::addHealth(unsigned int amount) {
  this->lives += amount;
  logger.logEvent(Events::PLAYER_HEALTH_ADDED, NULL);
}
