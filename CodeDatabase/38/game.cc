#include "game.h"
#include "areadamage.h"
#include "defs.h"
#include "directdamage.h"
#include "enemy.h"
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unistd.h>
void Game::setupController() {}
Game::Game() : hand(10), gc() {
  width = 20;
  height = 20;
  cellwidth = SCREENW / width;
  cellheight = SCREENH / height;
  drawer = new Visualizer<RaylibRender>(width, height, cellwidth, cellheight);
  bool players_turn = false;

  player = new Player(0, 7, logger);

  field = new Field(width, height, *player);
  // raylibrenderer = new RaylibRender(width, height, cellwidth, cellheight);
  field->updateField(*player, enemies.enemies);
  InitWindow(REALW, SCREENH, "Game");
  SetTargetFPS(60);
  setupController();
  // enemies.emplace_back(10, 10, Enemy::DIRECT);
  // enemies.emplace_back(5, 10, Enemy::PREDICT);
  // enemies.emplace_back(2, 5, Enemy::AMBUSH);
  // enemies.emplace_back(7, 6, Enemy::WANDER);
  // hand.addCard(Spell::AREA);

  while (!WindowShouldClose() && loop()) {
  }
  CloseWindow();
}
bool Game::loop() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<std::mt19937::result_type> distEnemySpawner(0,
                                                                            3);
  std::uniform_int_distribution<std::mt19937::result_type> distX(0, width - 1);
  std::uniform_int_distribution<std::mt19937::result_type> distY(0, height - 1);

  std::uniform_int_distribution<std::mt19937::result_type> distSpellSpawner(0,
                                                                            2);
  std::vector<std::pair<int, int>> persistent_coords;

  field->updateField(*player, enemies.enemies);

  traps.erase(std::remove_if(traps.begin(), traps.end(),
                             [](const Trap &t) { return !t.isAlive(); }),
              traps.end());

  towers.erase(std::remove_if(towers.begin(), towers.end(),
                              [](const EnemyTower &t) { return !t.isAlive(); }),
               towers.end());

  // PLAYER'S TURN TO MOVE
  if (round_num % 2 == 0) { // player turn modulus 2
    round_num++;
    // Player::PlayerMoves move = player->move_player(*field, *this);
    Player::PlayerMoves move = gc.move(*field, *this, *player);
    switch (move) {
    case Player::NOTHING:
      round_num--;
      break;

    case Player::MOVED: {
      Metadata *meta = static_cast<Metadata *>(malloc(sizeof(Metadata)));
      meta->x = player->getX();
      meta->y = player->getY();
      logger.logEvent(Events::PLAYER_MOVED, meta);
      free(meta);
    } break;

    case Player::DIRECTSPELL:

      // logger.logEvent("PLAYER", "Direct Spell used!");
      logger.logEvent(Events::SPELL_CAST, NULL);
      if (!hand.useSpell(Spell::DIRECT, persistent_str, directdamage,
                         areadamage, player, enemies, field))
        round_num--;

      else if (!directdamage.doDamage(*player, enemies.enemies, *field)) {
        round_num--;
        // DrawText(TextFormat("Cannot use direct attack! YOUR TURN"), 820,
        // 200+50*4,48,RED);
        persistent_str = "Cannot use direct attack! \nYOUR TURN";

      } else {

        // DrawText(TextFormat("Attack successful!"), 820, 200+50*4,48,RED);
        persistent_str = "Attack Successful!";
      }
      break;

    case Player::AREASPELL:
      logger.logEvent(Events::AREA_SPELL, NULL);

      if (!hand.useSpell(Spell::AREA, persistent_str, directdamage, areadamage,
                         player, enemies, field)) {
        round_num--;
      } else {

        highlightArea(player->getX(), player->getY(), 2);
      }

      break;

    case Player::TRAPSPELL: {
      logger.logEvent(Events::TRAP_PLACED, NULL);

      bool placed = false;
      if (hand.getCardAmount(Spell::TRAP) == 0) {
        persistent_str = "0 Trap cards!\nYOUR TURN";
        round_num--;
      } else {
        if (!field->isValidCoord(player->getX(), player->getY())) {
          round_num--;
          persistent_str = "Trap already there!\nYOUR TURN!";
          placed = true;
        }
        for (auto &t : traps) {
          if (t.getX() == player->getX() && t.getY() == player->getY() &&
              t.isActive()) {
            round_num--;
            persistent_str = "Trap already there!\nYOUR TURN!";
            placed = true;
          }
        }
        if (placed == false) {
          hand.useSpell(Spell::TRAP, persistent_str, directdamage, areadamage,
                        player, enemies, field);
          traps.emplace_back(player->getX(), player->getY());
          persistent_str = "Trap placed!";
        }
      }
    } break;

    case Player::REJECT_MOVE:
      persistent_str = "Cannot move there.\nYOUR TURN";
      round_num--;

      break;

    case Player::UNSET:
      throw std::invalid_argument("How did we get here");
      break;
    }
  }
  // ENEMIES TURN TO MOVE
  else { // enemy turn
    round_num++;
    for (auto &tower : towers) {
      tower.resetAttack();
      tower.tryAttack(*player, *field, directdamage, logger);
    }
    enemies.updateEnemies(*field, *player, traps, logger);
  }

  // SPAWNERS

  if (round_num % 50 == 0 && round_num > 0) { // Every 25 rounds, spawn a tower

    int tx = distX(mt);
    int ty = distY(mt);

    while (!field->isCellEmpty(tx, ty)) {
      tx = distX(mt);
      ty = distY(mt);
      logger.logEvent(Events::TOWER_SPAWNED, NULL);
    }

    towers.emplace_back(tx, ty); // default range=3, damage=10
    round_num++;
  }

  if (round_num % 20 == 0) { // SPAWN SPELLS EVERY 10 ROUNDS
    int type = distSpellSpawner(mt);
    hand.addCard(static_cast<Spell::SpellType>(type));
    logger.logEvent(Events::CARD_ADDED, NULL);
    player->addHealth(5);
    logger.logEvent(Events::PLAYER_HEALTH_ADDED, NULL);
    round_num++;
  }

  // SPAWN NEW ENEMY EVERY 15 round_numS
  if (round_num % 30 == 0) {
    enemies.addEnemy(*field, mt, distX, distY, distEnemySpawner);
    logger.logEvent(Events::ENEMY_SPAWNED, NULL);
    round_num++;
  }

  /////////////////////////////////

  // if(round_num % 50 == 0)move_enemies();
  for (auto &tower : towers) {
    tower.tryAttack(*player, *field, directdamage, logger);
  }

  if (!player->isAlive()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("YOU DIED!", 280, 350, 128, RED);
    logger.logEvent(Events::GAME_ENDED, NULL);
    EndDrawing();
    sleep(5);
    CloseWindow();
    return false;
  }

  // Check if player is trapped and has no spells
  bool canMove = !player->isTrapped(*field, hand);
  // If the player is trapped and has no spells left → Game Over
  if (!canMove) {
    player->drawGameOverTrapped();
    return false;
  }

  BeginDrawing();
  ClearBackground(BLACK);

  // mainDrawLoop();
  // raylibrenderer->render(player, field, hand, enemies, traps, towers,
  // squareHighlight);
  drawer->render_call(player, field, hand, enemies, traps, towers,
                      squareHighlight);

  EndDrawing();
  return true;
}
/*
void Game::move_enemies() {
  int targetX = player->getX();
  int targetY = player->getY();

  field->updateField(*player, enemies.enemies);
}
*/
void Game::removeHighlightArea() {
  squareHighlight.clear();
  Game::area_dmg_active = false;
}
void Game::highlightArea(unsigned int centerX, unsigned int centerY,
                         unsigned int radius) {
  removeHighlightArea();
  area_dmg_active = true;

  // highlight a 2×2 zone (centered on the player)
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      int x = centerX + dx;
      int y = centerY + dy;
      if (field->isValidCoord(x, y)) {
        squareHighlight.emplace_back(x, y);
      }
    }
  }
}

/*void Game::mainDrawLoop() {
  // ui.drawUI(SCREENW, SCREENH, cellwidth, cellheight, persistent_str,
  // *player);
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
  for (auto &[hx, hy] : Game::squareHighlight) {
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
}*/
