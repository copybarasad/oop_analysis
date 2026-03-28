#ifndef RPG_RENDERER_H_
#define RPG_RENDERER_H_

#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Entity.h"
#include "Field.h"
#include "Player.h"
#include <iostream>

class Renderer {
private:
  const Field &field;
  const Player &player;
  const EntityManager<Enemy> &enemyManager;
  const EntityManager<EnemyBuilding> &buildingManager;
  const void *targetingStatePtr;
  const std::string *messagePtr;

public:
  Renderer(const Field &field, const Player &player,
           const EntityManager<Enemy> &enemyManager,
           const EntityManager<EnemyBuilding> &buildingManager)
      : field(field), player(player), enemyManager(enemyManager),
        buildingManager(buildingManager), targetingStatePtr(nullptr),
        messagePtr(nullptr) {}
  void render() const;
  void setTargetingStatePtr(const void *ptr) { targetingStatePtr = ptr; }
  void setMessagePtr(const std::string *ptr) { messagePtr = ptr; }
  void printVictoryMessage() const;

private:
  void clearScreen() const;
  void printStatus() const;
  void printFieldBorder() const;
  void printCombatMode() const;
  void printTargetingMessage() const;
  void printLegend() const;
  void printCommands() const;
  void printHand() const;
  void printMessage() const;
};

#endif // RPG_RENDERER_H_
