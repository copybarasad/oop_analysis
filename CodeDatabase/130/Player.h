#ifndef PLAYER_H
#define PLAYER_H

#include "GameTypes.h"
#include "Position.h"

class Player {
private:
    int health;
    int maxHealth;
    int meleeDamage;
    int rangedDamage;
    int score;
    CombatMode mode;
    bool canMove;
    bool slowed;
    Pos position;

public:
    explicit Player(int hp = 100, int meleeDmg = 20, int rangedDmg = 15);

    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getScore() const;
    CombatMode getMode() const;
    bool canPlayerMove() const;
    bool isPlayerSlowed() const;
    Pos getPosition() const;

    void takeDamage(int dmg);
    void heal(int amount);
    void addScore(int points);
    void switchMode();
    void setMoveAllowed(bool allowed);
    void setSlowed(bool isSlowed);
    void resetMovement();
    void setPosition(int x, int y);
    void setPosition(const Pos& pos);
    
    // Upgrade methods
    void increaseMaxHealth(int amount);
    void increaseDamage(int amount);
    void restoreFullHealth();
    
    // Save/Load methods
    void setMaxHealth(int hp);
    void setHealth(int hp);
    void setDamage(int dmg);

    bool isAlive() const;
};

#endif