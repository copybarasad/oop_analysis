#pragma once
#include "Diver.h"
#include "Enums.h"
#include "Divers_bag.h"


class Player: public Diver {
private:
    int level;
    int score;
    AttackType currentAttackType;
    SpellFlag currentSpellType;
    int commonDamage;
    int rangedDamage;
    int rangedRange;
    bool canAttack;
    Divers_bag bag;
    
public:
    Player(GameConfig& cfg);
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    int getLevel();
    int getScore();
    AttackType getAttackType() const;
    std::string getAttackTypeString() const;
    int getRangedRange() const;
    bool canAttackThisTurn() const;
    void resetAttack();
    void switchAttackType();
    void performAttack();
    void addLevel();
    void addScore();
    Divers_bag& getBag();
    bool spendScoreOnSpell(int amount);
    void displayBag();
    void getInfo();
    void reset();

    void saveTo(SaveFileRAII& out) const;
    void loadFrom(SaveFileRAII& in);
};