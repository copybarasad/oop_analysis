#ifndef ENEMY_H
#define ENEMY_H

#include <vector>

class Player;
class GameField;

class Enemy {
public:
    Enemy(int x, int y);
    
    void move(Player& player, std::vector<Enemy>& allEnemies, GameField& field);
    void attack(Player& player) const;
    void takeDamage(int damage);
    bool isAlive() const;
    
    int getHealth() const;
    int getX() const;
    int getY() const;
    int getDamage() const;
    
    void setHealth(int newHealth);
    void setDamage(int newDamage);

private:
    int health;
    int damage;
    int x;
    int y;
};

#endif