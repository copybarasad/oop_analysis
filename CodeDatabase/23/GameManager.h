#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Constants.h"
#include "GameField.h"
#include "GameSaveLoad.h"
#include "GameState.h"
#include "Player.h"
#include <memory>
#include <string>
#include <vector>


class ConsoleInputReader;
class Spell;

enum class GameCommand {
  MOVE_UP,
  MOVE_LEFT,
  MOVE_DOWN,
  MOVE_RIGHT,
  ATTACK_LEFT,
  ATTACK_UP,
  ATTACK_RIGHT,
  ATTACK_DOWN,
  SWITCH_ATTACK,
  CAST_SPELL,
  SKIP,
  SAVE,
  HELP,
  QUIT,
  UNKNOWN
};

enum class LevelResult { IN_PROGRESS, LEVEL_COMPLETE, PLAYER_DIED, QUIT };

class GameManager {
private:
  std::unique_ptr<GameField> field;
  int currentLevel;
  bool gameRunning;

  
  int getLevelFieldSize(int level) const;
  int getLevelEnemyCount(int level) const;
  int getLevelEnemyHpBonus(int level) const;
  int getLevelEnemyDmgBonus(int level) const;

  
  GameCommand parseCommand(const std::string &cmd);
  bool handlePlayerMove(int dx, int dy);
  bool handlePlayerAttack(int dx, int dy);
  bool handlePlayerCast();
  void processEnemyTurn();
  void processBuildingsTurn();
  void processTowersTurn();
  LevelResult checkLevelCompletion() const;

  
  void startLevel(int level);
  void levelComplete();
  void playerDied();

  
  void showUpgradeMenu();
  void applyUpgrade(char choice);
  void removeHalfSpells(Player *player);
  void capturePlayerState();
  void applySavedPlayerState();
  Spell *createSpellByName(const std::string &name);

  
  void handleSave();
  void handleLoad();
  bool askToLoad();
  bool askToRestart();

  
  void printHelp();
  void displayLevelInfo() const;

  
  bool hasSavedPlayerState;
  int savedPlayerHp;
  int savedPlayerBaseDamage;
  int savedPlayerHandCapacity;
  int savedPlayerScore;
  AttackType savedAttackType;

  
  std::unique_ptr<ConsoleInputReader> inputReader;
  std::vector<std::string> savedSpells;

public:
  GameManager();
  ~GameManager();

  void startGame();
  void runLevel(int level);
  bool processPlayerTurn();
  void processEnemyTurns();
  LevelResult updateGame();

  
  bool executeMove(int dx, int dy);
  bool executeAttack(int dx, int dy);
  bool executeSwitchAttack();
  bool executeCastSpell();
  void executeSave();
  void executeHelp();
  void executeQuit();

  
  GameField *getField() { return field.get(); }
  const GameField *getField() const { return field.get(); }
  bool isGameRunning() const { return gameRunning; }
  int getCurrentLevel() const { return currentLevel; }

  
  void saveGame(const std::string &filename = GameSaveLoad::DEFAULT_SAVE_FILE);
  void loadGame(const std::string &filename = GameSaveLoad::DEFAULT_SAVE_FILE);
  bool hasSaveFile(
      const std::string &filename = GameSaveLoad::DEFAULT_SAVE_FILE) const;
};

#endif 
