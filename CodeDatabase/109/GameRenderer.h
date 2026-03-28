#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "Game.h"
#include <iostream>

class GameRenderer {
public:
  virtual void render(const Game& game) = 0;
  virtual void renderMenu(const std::vector<std::string>& options) = 0;
  virtual void renderMessage(const std::string& message) = 0;
  virtual ~GameRenderer() = default;
};

class ConsoleRenderer : public GameRenderer {
public:
  void render(const Game& game) override;
  void renderMenu(const std::vector<std::string>& options) override;
  void renderMessage(const std::string& message) override;
  
private:
  void renderPlayerInfo(const Player& player);
  void renderMap(const GameMap& map);
};

#endif