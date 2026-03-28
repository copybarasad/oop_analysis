#pragma once

class Enemy {
private:
    int health;
    int damage;
    
public:
    Enemy(int enemyHealth, int enemyDamage);
    
    int getHealth() const;
    int getDamage() const;
    bool isAlive() const;
    
    void takeDamage(int damage);
};