#ifndef PLAYER_H
#define PLAYER_H

#include "character.hpp"

enum WeaponType {
    MELEE,
    RANGED
};

class Player : public Character {
    char icon = 'P';
    int score = 0;
    WeaponType weapon = MELEE;
    int range = 1;
    bool slowed = false; 
    
public:
    Player(int startHealth, int startDamage, int startX, int startY);
    char getIcon() override { return icon; }
    
    void addScore(int points);
    int getScore();
    WeaponType getWeapon();
    void setWeapon(WeaponType newWeapon);
    int getRange();
    void setRange(int newRange);
    void setScore(int newScore){score=newScore;}
    
    bool isSlowed() const { return slowed; }
    void setSlowed(bool value) { slowed = value; }
    void clearSlow() { slowed = false; }

    void switchWeapons();
};

#endif