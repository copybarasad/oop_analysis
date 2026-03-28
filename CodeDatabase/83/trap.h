#ifndef TRAP_H
#define TRAP_H

#include <string>

class Enemy;

class Trap {
public:
    Trap(int x, int y, const std::string& spellName, int damage);
    
    int getX() const { return x; }
    int getY() const { return y; }
    const std::string& getSpellName() const { return spellName; }
    int getDamage() const { return damage; }
    bool isActive() const { return active; }
    void trigger() { active = false; }
    
    bool checkTrigger(int enemyX, int enemyY, Enemy& enemy);

private:
    int x, y;
    std::string spellName;
    int damage;
    bool active;
};

#endif