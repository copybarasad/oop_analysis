#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "PlayerHand.h"

class Field;
class Enemy;
class EnemySpawner;
class EnemyTower;   

enum class CombatMode { MELEE, RANGED };
enum class MoveResult { SUCCESS, SUCCESS_SLOWED, WALL, OCCUPIED, INVALID_POSITION };

class Player {
public:
    Player(int startX, int startY, int health, int meleeDamage, int rangedDamage);
    
    MoveResult move(int dx, int dy, Field& field);
    void attack(int dx, int dy, Field& field, 
                std::vector<Enemy>& enemies, 
                std::vector<EnemySpawner>& spawners, 
                std::vector<EnemyTower>& towers);

    void switchMode();
    void processTrap();
    void takeDamage(int amount);
    bool isAlive() const;
    void addScore(int points);
    bool spendScore(int amount);

    int getX() const;
    int getY() const;
    int getHealth() const;
    int getScore() const;
    int getDamage() const;
    std::string getModeString() const;
    CombatMode getMode() const;
    bool isSlowedThisTurn() const;
    
    PlayerHand* getHand() const;
    void addBuffCharge();
    int getBuffCharges() const;
    void consumeBuffCharges();
    
    void save(std::ofstream& out) const;
    void load(std::ifstream& in);
    void teleport(int newX, int newY);
    
    void healFull();
    void upgradeDamage(int amount);
    void upgradeHealth(int amount);

    void reset();

private:
    void updatePosition(int newX, int newY, Field& field); 
    int x, y;
    int health, maxHealth, score;
    int meleeDamage, rangedDamage, attackRange;
    int baseHealth;
    int baseMelee;
    int baseRanged;

    CombatMode currentMode;
    bool isSlowed;
    
    std::unique_ptr<PlayerHand> hand;
    int buffCharges;
};

#endif