#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../Characters/Enemy.h"
#include "../Characters/EnemyBuilding.h"
#include "../Characters/Player.h"
#include "../Characters/Ally.h"
#include "../Characters/Trap.h"
#include "../Characters/EnemyTower.h"
#include "../Spells/Hand.h"
#include "MovementSystem.h"

class GameState{
private:
    Hand hand;
    Field field;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<EnemyBuilding> builds;
    std::vector<Ally> allies;
    std::vector<Trap> traps;
    std::vector<EnemyTower> towers;
    MovementSystem movSys;

public:
    GameState(Field field, Hand hand, Player player);

    Field& getField();
    Player& getPlayer();
    std::vector<Enemy>& getEnemies();
    std::vector<EnemyBuilding>& getBuilds();
    std::vector<Ally>& getAllies();
    std::vector<Trap>& getTraps();
    std::vector<EnemyTower>& getTowers();
    Hand& getHand();
    void setHand(Hand hand);

    void deleteDead();
    bool isGameOver() const;
    bool isWin() const;

    int findEnemy(std::pair<int, int> position) const;
    int findEnemyBuild(std::pair<int, int> position) const;
    int findAlly(std::pair<int, int> position) const;
    int findTower(std::pair<int, int> position) const;
    Character* findCharacter(std::pair<int, int> position);
};

#endif