#ifndef PLAYER_H
#define PLAYER_H
#define MELEE_ATTACK_RANGE 1
#define RANGED_ATTACK_RANGE 3

#include <vector>

class Field; 
class Tower;
class Enemy;

class Player {
private:
    int x, y;
    int Health;
    bool Alive;
    int meleedamage;
    int rangedamage;
    bool Ismelee;

public:
    Player(int xStart, int yStart, int hp, int meleedm, int rangedm);
    
    void move(char button, const Field& field);
    void switch_attack();
    void takeDamage(int dmg);
    int getDistanceTo(int targetX, int targetY) const;
    bool attack(std::vector<Enemy>& enemies);
    bool attackTower(Tower& tower);
    void heal(int amount);

    int getHealth() const;
    int getCurrentDamage() const;
    int getX() const;
    int getY() const;
    bool isAlive() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    bool isMeleeMode() const;
};

#endif