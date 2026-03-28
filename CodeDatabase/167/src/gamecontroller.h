#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <memory>
#include <string>
#include "gamefield.h"
#include "player.h"
#include "gamesave.h"

class GameController
{
private:
  enum class GameState
  {
    MAIN_MENU,
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER,
    SAVING,
    LOADING,
    EXITING
  };

  std::unique_ptr<GameField> gameField;
  std::unique_ptr<Player> player;
  GameSave gameSave;
  GameState currentState;
  int currentLevel;
  bool isRunning;

  const int ACTION_PAUSE_MS = 500;

  void mainLoop();
  void processInput(char input);

  void showMainMenu();
  void showSaveMenu();
  void showLoadMenu();
  void showGameOverMenu();
  void showLevelCompleteMenu();

  void clearScreen() const;
  char getCharWithoutEnter() const;
  void sleepMs(int milliseconds) const;
  void displayGameStatus() const;
  std::string generateSaveName() const;

  void initializeLevel(int level);
  bool isLevelComplete() const;
  void advanceToNextLevel();

  void playerTurn(char input);
  void enemyTurn();

public:
  GameController();
  ~GameController();

  void start();
  void stop();

  void saveGame(const std::string &saveName);
  void loadGame(const std::string &saveName);
  void newGame();
  void restartLevel();

  bool isGameRunning() const { return isRunning; }
  int getCurrentLevel() const { return currentLevel; }
};

#endif