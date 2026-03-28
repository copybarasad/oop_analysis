#pragma once

#include <cmath>
#include <memory>
#include "Character.h"
#include "../spells/DirectFightSpell.h"

class Fortress : public Character {
private:
    int attackFrequency;
    int moveCounter;
    std::unique_ptr<DirectFightSpell> spell;
public:
    Fortress(int x = 0, int y = 0,int health = 40, int attackRad = 2, int damage = 4, int attackFrequency = 3);
    ~Fortress() = default;
    bool attackIsReady();
    void updateCounter();
    DirectFightSpell& getFortressSpell() {return *spell; };
    int getAttackFrequency() { return attackFrequency; }
    int getAttackRadius() { return spell->getCurrentR(); }
    int getDamage() { return spell->getDamage();}
};