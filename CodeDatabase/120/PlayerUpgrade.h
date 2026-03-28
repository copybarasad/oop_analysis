#pragma once
#include <iostream>

#include "Player.h"

class PlayerUpgrade {
 public:
  PlayerUpgrade(Player& p, GameLogic& log);
  void ShowUpgradeMenu();

 private:
  Player& player;
  GameLogic& logic;
};
