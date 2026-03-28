#pragma once
#include "Position.h"
#include "Hand.h"
#include "Spell.h"
#include "BuffState.h"
#include "Game.h"
#include <vector>
#include <iostream>


class Hand;
class Spell;
class Game;
class BuffState;

class Player {
public:
    Player(Position pos, size_t handCapacity, int hp, int coins);

    Position GetPosition() const;
    void SetPosition(Position p);
    
    bool Move(Position delta, Game const& game);

    Hand& GetHand();
    const Hand& GetHand() const;

    void ApplyAccumulatedBuffsToSpell(Spell& spell);
    void AddBuff(BuffState const& b);
    const BuffState& GetAccumulatedBuff() const;
    void ClearBuffs();

    int  GetCoins() const;
    void AddCoins(int v);
    bool SpendCoins(int v);

    void CollectCoinsFromGame(Game& game);


    int  GetHp() const;
    void TakeDamage(int dmg);
    bool IsAlive() const;

private:
    Position pos_;
    Hand hand_;
    //std::vector<BuffState> accumulatedBuffs_;
    BuffState accumulatedBuff_;
    int coins_;

    int hp_;
    int maxHp_;
};