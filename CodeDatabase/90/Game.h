#ifndef RPG_GAME_H_
#define RPG_GAME_H_

#include "CombatSystem.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EntityManager.h"
#include "Field.h"
#include "InputHandler.h"
#include "Player.h"
#include "Position.h"
#include "Random.h"
#include "Renderer.h"
#include "SaveSystem.h"
#include <memory>

class Game {
private:
  Field field;
  Player player;

  EntityManager<Enemy> enemyManager;
  EntityManager<EnemyBuilding> buildingManager;

  std::unique_ptr<CombatSystem> combatSystem;
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<InputHandler> inputHandler;

  bool gameRunning;
  int totalEnemiesDefeated = 0;

  void updateEnemies();
  bool isGameOver() const;
  bool isVictory() const;
  bool shouldEnd() const;
  void stopGame();
  bool isOccupied(const Position &pos, const Entity *ignore = nullptr) const;
  Position findPassableSpawnNear(Position startPos) const;

public:
  bool movePlayer(Position newPos);
  Game(int fieldWidth = 15, int fieldHeight = 15);
  void run();
  void update();
  const Field &getField() const { return field; }
  Field &getField() { return field; }
  const Player &getPlayer() const { return player; }
  Player &getPlayer() { return player; }
  const EntityManager<Enemy> &getEnemyManager() const { return enemyManager; }
  EntityManager<Enemy> &getEnemyManager() { return enemyManager; }
  const EntityManager<EnemyBuilding> &getBuildingManager() const {
    return buildingManager;
  }
  EntityManager<EnemyBuilding> &getBuildingManager() { return buildingManager; }
  std::unique_ptr<CombatSystem> &getCombatSystem() { return combatSystem; }
  std::unique_ptr<Renderer> &getRenderer() { return renderer; }
  std::unique_ptr<InputHandler> &getInputHandler() { return inputHandler; }
  int getTotalEnemiesDefeated() { return totalEnemiesDefeated; }
  void setGameRunning(bool running) { gameRunning = running; }
};

#endif // RPG_GAME_H_