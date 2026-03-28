#ifndef HEROH
#define HEROH
#include "../abstractNinterfaces/pawn.h"
#include "../spells/spell.h"
#include <vector>
#include "../spells/hand.h"
#include "../abstractNinterfaces/decision.h"
class hero : public pawn, public decision{
    size_t maxHP;
    size_t stance;
    size_t MP;
    hand inventory;
    size_t menuFlag;
    size_t magicalMastery;
public:
    void environmentInteraction(map*, coords);
    void makeTurn(map*);
    void buyUpgrades(map*);
    void checkYourself();
    hero(int, size_t, size_t);
    hero(int, size_t);
    hero(int);
    hero();
    void changeStance();
    void setStance(size_t);
    size_t getStance();
    void setMP(size_t);
    size_t getMP();
    void setMaxHP(size_t);
    size_t getMaxHP();
    hand& getInventory();
    void changeMenuFlag();
    size_t getMenuFlag();
    void setMagicalMastery(size_t);
    size_t getMagicalMastery();
    ~hero();
};
#endif