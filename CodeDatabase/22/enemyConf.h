#include <string>

struct en_conf {
    int enemyHealth = 100;
    int enemyHutHealth = 100;
    int towerHealth = 100;

    int enemyDamage = 10;
    
    int towerDamage = 10;
    std::string towerName = "";
    std::string towerDescription = "";
    int towerRadius = 2;

    int numEnemy = 1;
    int numEnemyHut = 1;
    int numTower = 1;
};