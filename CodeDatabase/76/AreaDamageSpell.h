#pragma once
#include "SpellCard.h"
#include "Player.h"    
#include "Enemy.h"     
#include <string>  

class AreaDamageSpell : public SpellCard {
private:
    string name;
    int damage;
    int manaCost;
    int range;

public:
    AreaDamageSpell(string spellName = "Bomb", int dmg = 10, int mana = 15, int rng = 2);
    
    string GetName() const override;
    string GetDescription() const override;
    int GetManaCost() const override;
    bool CanCast(const Player& caster, Enemy& enemy) const override;
    void Cast(Player& caster, Enemy& enemy) override;
};