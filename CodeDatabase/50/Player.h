#pragma once
#include <iostream>
#include "GameEnt.h"
#include "GameExceptions.h"


class Player : public GameEnt{

private:
    bool rangedAttack;   // true — дальний, false — ближний
    int rangedAttackRange;
    double rangeDamage;
    double exp;
    int mana;
    double maxHealth;   
    int maxMana;   

public:
    
    
    Player (double playerHealth= 10, double playerDamage = 2, double playerRangeDamage = 10, int playerMana = 5, double playerExp = 0, int pos_x = 0, int pos_y = 0);
    
    ~Player() = default;

    int getAttackRange() const { return rangedAttackRange; }

    double getPlayerDamage() const;

    bool attackIsRange() const {return rangedAttack;};

    int getMana() const {return mana;};

    GameEnt* clone() const override {return new Player(*this);}

    void healToFull() { health = maxHealth; }
    void manaToFull() { mana = maxMana; }

    //LVLUP
    void increaseMaxHealth(double amount);
    void increaseMeleeDamage(double amount);
    void increaseRangeDamage(double amount);
    void increaseMaxMana(int amount);

    // Не константные методы

    void switchAttackType(){rangedAttack = !rangedAttack;};

    void reduceMana(int amount);

    //save and load
    void save(std::ostream& out) const;
    void load(std::istream& in);

};