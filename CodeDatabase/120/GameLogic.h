#pragma once
#include <queue>
#include "EventSystem.h"
#include "GameWorld.h"

class GameLogic {
 private:
  GameWorld& world;
  bool gameOver = false;
  bool levelCleared = false;
  public:
  EventSystem& events;
  EventSystem& GetEventSystem() { return events; }
  const EventSystem& GetEventSystem() const { return events; }
  explicit GameLogic(GameWorld& world, EventSystem& es);
  bool PlayerCastSpell(size_t handIndex,
                       const std::optional<Position>& targetPos);
  void MovePlayer(int dx, int dy);
  void PlayerAttack();
  void UpdateTurn();
  bool IsGameOver() const;
  bool WasLevelCleared() const;
  void ClearLevelClearedFlag();
  void ResetGameOverFlag();  
 private:
  void UpdateEnemies();
  void UpdateSpawners();
};
