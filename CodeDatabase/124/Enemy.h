#ifndef ENEMY_H
#define ENEMY_H

class Player;
class GameField;
class Enemy {
private:
    int baseHealth;
    int baseDamage;
    int health;
    int damage;
    int level;
    bool isActive;

public:
    Enemy(int enemyLevel = 1);
    
    int getHealth() const;
    int getDamage() const;
    bool getIsActive() const;
    bool isAlive() const;
    int getLevel() const;
    
    void takeDamage(int damage);
    void setActive(bool active);   
    void setLevel(int newLevel);
     
    void setHealth(int newHealth);
    
    void moveTowardsPlayer(GameField& field, Player& player);
    
private:
    bool tryAttackPlayer(const GameField& field, int playerX, int playerY, Player& player);
};

#endif