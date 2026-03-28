#pragma once
#include "SpellCard.h"
#include <vector>
#include <memory>
#include <random>

class SpellHand 
{
private:
    vector<shared_ptr<SpellCard>> spells;
    size_t maxSize;
    int enemiesKilled;
    mt19937 rng;

    shared_ptr<SpellCard> CreateRandomStarterSpell();
    void AddRandomStarterSpell();

public:
    SpellHand(size_t maxSpells = 5);
    
    bool AddSpell(shared_ptr<SpellCard> spell);
    bool CanCastSpell(int index, const Player& caster, Enemy& enemy) const;
    void CastSpell(int index, Player& caster, Enemy& enemy);
    void DisplaySpells(const Player& player, Enemy& enemy) const;
    void RemoveSpell(int index);
    size_t GetSpellCount() const;
    bool IsFull() const;
    void Clear();
    
    bool BuySpell(shared_ptr<SpellCard> spell, int cost, Player& player);
    void AddRandomSpell();
    shared_ptr<SpellCard> GetSpell(int index) const;
};