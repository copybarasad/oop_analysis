#pragma once
#include "SpellCard.h"
#include "PlayingField.h"
#include "Enemy.h"
#include <memory>

using namespace std;

class TrapSpell : public SpellCard {
private:
    string name;
    string description;
    int manaCost;
    int damage;

public:
    TrapSpell(string spellName = "Bear Trap", int damage = 15, int mana = 10);
    
    string GetName() const override;
    string GetDescription() const override;
    int GetManaCost() const override;
    int GetDamage() const;
    
    bool CanCast(const Player& caster, Enemy& enemy) const override;
    void Cast(Player& caster, Enemy& enemy) override;
    
    static void CheckTraps(PlayingField& field, Enemy& enemy);
};