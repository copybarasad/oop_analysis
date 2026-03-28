#ifndef ENEMY_H
#define ENEMY_H

class Position;
class GameField;

class Enemy {
private:
    int health;
    const int damage;

public:
    Enemy(int initialHealth, int enemyDamage);
    virtual ~Enemy() = default;
    
    void takeDamage(int damage);
    bool isAlive() const;
    int getDamage() const;
    int getHealth() const;
    
    virtual bool isBuilding() const;
};

#endif