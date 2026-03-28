#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Hand.h"
#include "Spell.h"

class Player: public Character{
public:
    Player(int hp, int meleeDamage, int rangedDamage, std::pair<int, int> pos);
    int score();
    void addScore(int value);
    bool spendScore(int value);

    void step(std::pair<int, int> delta);

    void AttackMobile();
    bool isMode();

    bool shouldSkipTurn();
    void setSkipNextTurn(bool v);

    Hand& hand();


    void AddPendingModifier(const SpellModifier& m);
    bool HasPendingModifier() const;
    SpellModifier PeekPendingModifier() const;
    SpellModifier TakePendingModifier();

private:
    int score_;
    bool rangedMode_;
    int meleeDamage_;
    int rangedDamage_;
    bool skipNextTurn_;

    Hand hand_;

    SpellModifier pendingMod_;
};

#endif