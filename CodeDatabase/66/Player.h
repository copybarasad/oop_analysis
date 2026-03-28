#ifndef PLAYER_H
#define PLAYER_H

#include "CombatMode.h"

class Player {
private:
    int health;
    int maxHealth;
    int meleeDamage;
    int rangedDamage;
    int score;
    CombatMode combatMode;
    bool isSlowed;

public:
    Player(int health, int meleeDamage, int rangedDamage);
    
    int getHealth() const;
    int getMaxHealth() const;
    int getCurrentDamage() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    int getScore() const;
    CombatMode getCombatMode() const;
    bool getIsSlowed() const;
    
    void takeDamage(int damage);
    void addScore(int points);
    void switchMode();
    void setSlow(bool slowed);
    
    bool isAlive() const;
};

#endif
