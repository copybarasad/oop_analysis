#ifndef FRAMEWORK_GAME_CONTROLLER_HPP
#define FRAMEWORK_GAME_CONTROLLER_HPP

#include "command.hpp"
#include "game.hpp"
#include <vector>

class Level;
class GameField;
class Player;
class Enemy;
class Hand;

template <typename TInput> 
class GameController {
public:
  GameController() = default;

  template <typename TVisualizer>
  void runMainMenu(Game &game, TVisualizer &visualizer, TInput &input) {
    bool running = true;
    while (running) {
      bool hasSave = hasSaveFile(game);
      visualizer.renderMainMenu(hasSave);

      MenuCommand cmd = input.readMenuCommand();
      switch (cmd.type) {
      case MenuCommandType::NewGame:
        game.startNewGame();
        break;
      case MenuCommandType::LoadGame:
        if (game.loadGame()) {
          game.continueLoadedGame();
        }
        break;
      case MenuCommandType::Exit:
        running = false;
        break;
      case MenuCommandType::Invalid:
      default:
        visualizer.renderInvalidMenuChoice();
        break;
      }
    }
  }

private:
  bool hasSaveFile(Game &game);
};

#endif