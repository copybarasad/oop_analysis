#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
private:
    int health;
    int damage;

public:
    Enemy(int health, int damage);
    Enemy(const Enemy& other);
    void setDamage(int newDamage);
    int getHealth() const;
    int getDamage() const;
    void takeDamage(int damage);
    bool isAlive() const;
};

#endif
