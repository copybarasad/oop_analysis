#ifndef ALLY_H
#define ALLY_H

class Ally {
private:
    int health;
    int damage;

public:
    Ally(int health = 30, int damage = 15);
    
    int getHealth() const;
    int getDamage() const;
    
    void takeDamage(int damage);
    bool isAlive() const;
};

#endif
