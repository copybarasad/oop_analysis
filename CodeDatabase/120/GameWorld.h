#pragma once
#include "GameMap.h"
#include "EnemySpawner.h"
#include "Player.h"
#include "SummonedAlly.h"
#include "Tower.h"
#include "Trap.h"

class GameWorld {
 public:
  GameWorld(int width, int height);
  int GetCurrentLevel() const;
  void SetCurrentLevel(int next);
  void InitEntities(int numEnemies = 2, int numSpawners = 1, int numTowers = 1);
  bool IsOccupied(int x, int y) const;
  GameMap& GetMap();
  Player& GetPlayer();
  std::vector<std::unique_ptr<Enemy>>& GetEnemies();
  std::vector<std::unique_ptr<EnemySpawner>>& GetSpawners();
  std::vector<std::unique_ptr<Tower>>& GetTowers();
  std::vector<std::unique_ptr<Trap>>& GetTraps();
  std::vector<std::unique_ptr<SummonedAlly>>& GetAllies();
  void AddTrap(std::unique_ptr<Trap> t);
  void AddSummonedAlly(std::unique_ptr<SummonedAlly> a);
  void ResetForNextLevel(int newWidth, int newHeight, int enemyCount,
                         int spawnerCount, int towerCount);

 private:
  GameMap map;
  Player player;
  std::vector<std::unique_ptr<Enemy>> enemies;
  std::vector<std::unique_ptr<EnemySpawner>> spawners;
  std::vector<std::unique_ptr<Tower>> towers;
  std::vector<std::unique_ptr<Trap>> traps;
  std::vector<std::unique_ptr<SummonedAlly>> allies;
  int currentLevel = 1;
};
