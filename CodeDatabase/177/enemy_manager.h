#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"
#include "game_field.h"
#include <vector>

class Player;
class CombatManager;

class EnemyManager {
private:
    std::vector<Enemy> enemies;
    GameField& field;
    int enemyHealth;  
    int enemyDamage; 
    void moveEnemy(Enemy& enemy, Player& player, CombatManager& combatManager);

public:
    explicit EnemyManager(GameField& field, int health = 15, int damage = 3);
    void setEnemyParams(int health, int damage);
    
    void spawnEnemy(const Enemy& enemy);
    const std::vector<Enemy>& getEnemies() const;
    
    std::vector<Enemy>& getEnemies() { return enemies; }
    
    void addEnemy(const Enemy& enemy);
    void processEnemyTurns(Player& player, CombatManager& combatManager); 
    void removeDeadEnemies(Player& player);
    void spawnEnemyAtPosition(const Position& pos);
    void clearEnemies();
    
    void processTrapDamage(const Position& position, int damage);
};

#endif