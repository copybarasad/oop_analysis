#ifndef TOWER_H
#define TOWER_H

#include "Player.h"
#include "Spell.h"
#include <memory>

class Field;
class Enemy;

class Tower {
private:
    int x, y;                 
    int range;               
    int health;               
    int cooldown;            
    int currentCooldown;     
    bool alive;             
    
    std::unique_ptr<Spell> weakSpell; 

public:
    Tower(int X, int Y, int range = 4, int health = 50, int cooldown = 3);

    void update(Player& player, std::vector<Enemy>& enemies, Field& field);
    void takeDamage(int damage);
    
    bool canAttack() const;
    bool playerInRange(const Player& player) const;
    
    int getX() const;
    int getY() const;
    int getHealth() const;
    int getRange() const;
    int getCooldown() const;
    int getCurrentCooldown() const;
    bool isAlive() const;
};

#endif