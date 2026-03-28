#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include <string>
#include <vector>
#include "player_hand.h"

class GameMap;
class Enemy;

class Player: public Entity {
private:
    enum class CombatMode { MELEE, RANGED };
    CombatMode combatMode;
    int meleeDamage;
    int rangedDamage;
    int attackRange; 
    int enemiesKilled;
    int enemiesForSpellReward; 
    static const int MELEE_ATTACK_RANGE = 1;
    static const int RANGED_ATTACK_RANGE = 2;
    PlayerHand hand;
    int maxHealth;

public:
    Player(int start_x = 0, int start_y = 0, int player_health = 100, int player_melee_damage = 100);
    ~Player()  = default; 
    bool move(int dx, int dy, GameMap& map);
    bool canRangedAttack(Position targetPos, int move_dx, int move_dy) const;
    int getAttackDamage() const;
    int getAttackRange() const { return attackRange; }
    bool isRangedMode() const { return combatMode == CombatMode::RANGED; }
    
    void switchCombatMode();
    std::string getCombatMode() const;
    std::string getStatusString() const override;
    void setAttackRange(int range) {
        attackRange = range;
    }
    PlayerHand& getHand();
    void onEnemyKilled();
    bool attack(int dx, int dy, GameMap& map);
    void onKill() override { hand.onEnemyKilled(enemiesKilled); }
    void update(int new_x, int new_y); 
    int getEnemiesKilled(){
        return enemiesKilled;
    }
    void setEnemiesKilled(int kill){
        enemiesKilled = kill;
    }

    void addMaxHealth(){
        maxHealth += 50;
    }

    int getMaxHealth(){
        return maxHealth;
    }

    int setMaxHealth(int health){
        return maxHealth = health;
    }

    int getdamage(){
        return meleeDamage;
    }
    void addDamage(){
        meleeDamage += 30;
        rangedDamage += 15;
    }
    void setDamage(int dam){
        meleeDamage = dam;
        rangedDamage = dam / 2;
    }
};

#endif