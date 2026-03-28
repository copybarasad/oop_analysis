#pragma once
#include "input.h"
#include "player.h"
class Field;
class Game;
template <typename Input> class GameControl {
public:
  // GameControl() : input() {
  // if (!input.loadConfig()) {
  // input.setDefaultMaps();
  // }
  // }

  Player::PlayerMoves move(Field &f, Game &g, Player &p) {
    COMMAND cmd = input.ProcessMove();
    return p.move_player(f, g, cmd);
  }

private:
  Input input;
};
