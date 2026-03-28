#pragma once
#include <vector>
#include <algorithm>    

#include <random> 
#include "Enemy.h"
#include "EnemyTower.h"
#include "GameExceptions.h"
#include "GameField.h"
#include "Player.h"



class EntityManager {
private:
    std::vector<Enemy> enemies;
    std::vector<EnemyTower> towers;

public:
    void initEnemies(Field& field, int count, int curLvl);
    int moveEnemies(Field& field, Player& player);
    int countAlive() const;
    void initTowers(Field& field, int count);
    void updateTowers(Field& field, Player& player);

    //save and load
    void save(std::ostream& out) const;
    void load(std::istream& in, Field& field);

};
