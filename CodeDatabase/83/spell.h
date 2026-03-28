#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <memory>
#include <vector>

class Enemy;
class Player;

class Spell {
public:
    virtual ~Spell() = default;
    
    virtual std::string getName() const = 0;
    virtual int getDamage() const = 0;
    virtual int getRange() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getLevel() const = 0;
    
    virtual void cast(int casterX, int casterY, int targetX, int targetY, 
                     std::vector<Enemy>& enemies, Player& player) = 0;
    
    virtual void display() const = 0;
    virtual std::unique_ptr<Spell> clone() const = 0;
    
    virtual void upgrade() = 0;
};

#endif