#ifndef BEHAVIOURSYSTEM_H
#define BEHAVIOURSYSTEM_H

#include "GameState.h"

class BehaviourSystem{
private:
    GameState& gameState;
    MovementSystem movSys;

public:
    BehaviourSystem(GameState& gameState);
    void spawnEnemy(EnemyBuilding& build);
    void updateBuild(EnemyBuilding& build);
    void enemyTurn(Enemy& enemy);
    void allyTurn(Ally& ally);
    void alliesTurn();
    void enemiesTurn();
    void buildingsTurn();
    void updateTraps();
};

#endif

