#ifndef ENEMY_H
#define ENEMY_H

#include <vector>

class Field;
class Player;

class Enemy {
private:
    int x, y;
    int Health;
    int Damage;
    bool Alive;

public:
    Enemy(int Xstart, int Ystart, int hp, int dm);
    
    void move(Player& player, Field& field, std::vector<Enemy>& allEnemies);
    void takeDamage(int dmg);
    
    int getX() const;
    int getY() const;
    int getHealth() const;
    int getDamage() const;
    bool isAlive() const;
};

#endif