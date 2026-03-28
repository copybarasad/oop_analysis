#ifndef AI_SYS_HPP
#define AI_SYS_HPP

class eManager;
class Entity;

class AISystem {
private:
    static void processEnemy(eManager& manager, Entity& enemy);

public:
    static void EnemyTurn(eManager& manager);
    static void TowerTurn(eManager& manager, int turn_count);
    
};

#endif
