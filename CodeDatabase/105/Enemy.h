#pragma once

class Enemy {
private:
    int health;
    int damage;
    int maxHealth;

public:
    Enemy(int hp = 30, int dmg = 5);
    
    int getHealth() const;
    int getDamage() const;
    int getMaxHealth() const;
        
    void takeDamage(int dmg);
    bool isAlive() const;
    
    // Для сериализации
    void setHealth(int hp);
    void setDamage(int dmg);
};

