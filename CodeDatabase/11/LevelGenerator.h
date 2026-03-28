#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <vector>
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h" 
#include "EnemyTower.h"   
#include "Trap.h"         
#include "Ally.h"         

class LevelGenerator {
public:
    static void generate(int difficulty, 
    Field& field, 
    Player& player, 
    std::vector<Enemy>& enemies,
    std::vector<EnemySpawner>& spawners,
    std::vector<Trap>& traps,
    std::vector<Ally>& allies,
    std::vector<EnemyTower>& towers);

private:
    static bool placeEntityRandomly(Field& field, CellType type, int& outX, int& outY);
};

#endif