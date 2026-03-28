#ifndef ALLY_H
#define ALLY_H

#include <vector>

class Field;
class Enemy;

class Ally {
public:
    Ally(int startX, int startY);
    void processTurn(Field& field, std::vector<Enemy>& enemies);
    bool isAlive() const;
    void takeDamage(int amount);
    int getX() const;
    int getY() const;

private:
    void move(int newX, int newY, Field& field);
    int x, y, health, damage;
};
#endif