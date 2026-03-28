#ifndef GAME
#define GAME

#include "cell.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "enemybuilding.h"
#include "enemyTower.h"
#include "ally.h"
#include "directDamageSpell.h"
#include "enhancementSpell.h"
#include <vector>
#include <string>
#include <memory>

enum class GameState {
  Starting, LevelRunning, LevelWon, LevelLost, Exiting
};

class GameException: public std::exception {
public:
  explicit GameException(const std::string& message);
  const char* what() const noexcept override;

private:
  std::string message;
};

class InvalidCommandException: public GameException {
public:
  explicit InvalidCommandException(const std::string& details);
};

class LevelLoadException: public GameException {
public:
  explicit LevelLoadException(const std::string& details);
};

class GameLogicException: public GameException {
public:
  explicit GameLogicException(const std::string& details);
};

class Game {
public:
  Game(int width, int height, int numEnemies);

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  void run();
  void processCommand(const std::string& input);

  const Field& get_field() const;
  const std::vector<Enemy>& get_enemies() const;
  const std::vector<EnemyBuilding>& get_buildings() const;
  const std::vector<EnemyTower>& get_towers() const;
  const std::vector<Ally>& get_allies() const;
  const Player& get_player() const;
  Player& get_player();

  void applyDamage(int x, int y, int damage);
  void placeTrap(int x, int y, int damage);
  void removeTrap(int x, int y);
  void addAlly(int x, int y);

  bool loadGame(const std::string& filename);

  static std::pair<bool, std::string> displayMainMenu();

private:
  void startGame();
  void loadLevel();
  void playerTurn(const std::string& command);
  void alliesTurn();
  void enemiesTurn();
  void buildingsAndTowersTurn();
  void updateField();
  void displayGameState() const;
  void handleGameOver();
  bool restartGame();
  void initializeEnemies(int numEnemies);
  void initializeBuildings(int numBuildings);
  void initializeTowers(int numTowers);
  void shootRanged(int dx, int dy);
  void castSpell(int spellIndex, int dx, int dy);
  void spawnByBuildings(int dx, int dy);
  void saveGame(const std::string& filename) const;

  bool isLevelWon() const;
  bool isLevelLost() const;

  GameState state;
  int currentLevel = 1;
  Field field;
  Player player;
  std::vector<Enemy> enemies;
  std::vector<EnemyBuilding> buildings;
  std::vector<EnemyTower> towers;
  std::vector<std::tuple<int, int, int>> traps;
  std::vector<Ally> allies;
  std::vector<std::vector<Cell>> initialGrid;
};

#endif