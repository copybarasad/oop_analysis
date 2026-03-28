#ifndef PLAYER_H
#define PLAYER_H

#include <string>

enum class CombatMode {
    MELEE,
    RANGED
};

class Player {
private:
    std::string name;
    int health;
    int maxHealth;
    int damage;
    int score;
    int positionX;
    int positionY;
    CombatMode combatMode;
    int attackRange;

public:
    Player(const std::string& playerName, int startX, int startY);
    
    std::string getName() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getScore() const;
    int getPositionX() const;
    int getPositionY() const;
    CombatMode getCombatMode() const;
    int getAttackRange() const;
    
    void setPosition(int x, int y);
    
    void takeDamage(int damage);
    void heal(int amount);
    void addScore(int points);
    void switchCombatMode();
    bool isAlive() const;
    
    bool move(int deltaX, int deltaY);
};

#endif