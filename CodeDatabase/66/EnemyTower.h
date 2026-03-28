#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

class EnemyTower {
private:
    int damage;
    int range;
    int cooldown;
    int currentCooldown;
    int health;
    int maxHealth;

public:
    EnemyTower(int damage = 15, int range = 2, int cooldown = 2, int health = 80);
    
    int getDamage() const;
    int getRange() const;
    bool canAttack() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getCooldownRemaining() const;
    void setCurrentCooldown(int cooldown);
    
    void processTurn();
    void attack();
    
    int takeDamage(int damage);
    bool isAlive() const;
};

#endif
