#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Hand.h"

enum class CombatMode {
    MELEE,
    RANGED
};

class Player : public Entity {
private:
    int meleeDamage;
    int rangedDamage;
    CombatMode combatMode;
    bool canMove;
    int money = 9999999;
    Hand hand;
public:
    Player(int startHealth = 100, int meleeDmg = 20, int rangedDmg = 15);
    
    int getDamage() const;
    CombatMode getCombatMode() const { return combatMode; }
    void setCombatMode(CombatMode mode) { combatMode = mode; }
    bool getCanMove() const { return canMove; }
    void setCanMove(bool can) { canMove = can; }
    int getMoney() const { return money; }
    void setMoney(int money);
    
    int getMeleeDamage() const { return meleeDamage; }
    void setMeleeDamage(int dmg) { meleeDamage = dmg; }
    int getRangedDamage() const { return rangedDamage; }
    void setRangedDamage(int dmg) { rangedDamage = dmg; }

    Hand& getHand() { return hand; }
    const Hand& getHand() const { return hand; }
    void switchCombatMode();
    
    std::string getEntityName() const override { return "Игрок"; }
};

#endif
