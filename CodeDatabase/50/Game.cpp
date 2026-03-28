#include "Game.h"

Game::Game()
    : turns(0),
      maxEnemCnt(3),
      maxTowerCnt(2),
      currentLevel(1),
      field(10,10),
      player(10, 2, 10, 15, 0, 4, 4),
      hand(3),
      battle(),
      entManager(){
    srand(static_cast<unsigned>(time(nullptr)));
}



void Game::resetState() {
    turns = 0;
    currentLevel = 1;
    player = Player(10, 2, 10, 15, 0, 4, 4);
    hand = PlayerHand(3);
}
