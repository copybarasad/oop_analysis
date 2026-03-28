#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "EnemyBuilding.h"

class EnemyTower : public EnemyBuilding {
private:
    int attackRadius;
    int attackDamage;
    int attackCooldown;
    int currentCooldown;
    bool canAttackThisTurn;

public:
    EnemyTower(int startX, int startY, int attackRadius = 5, 
               int attackDamage = 15, int attackCooldown = 2, int buildingId = 0);
    
    // Getters
    int getAttackRadius() const;
    int getAttackDamage() const;
    bool canAttack() const;
    
    // Атака игрока
    bool tryAttackPlayer(class Game& game, const class Player& player);
    
    // Обновление состояния
    void update();
    
    // Переопределение метода trySpawnEnemy (башни не спавнят врагов)
    bool trySpawnEnemy(class GameField& field, std::vector<class Enemy>& enemies) override;
    
    // Serialization
    std::string serialize() const;
    bool deserialize(const std::string& data);
};

#endif // ENEMYTOWER_H

