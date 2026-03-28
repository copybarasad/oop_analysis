#include "DirectDamageSpell.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>

using namespace std;

DirectDamageSpell::DirectDamageSpell(string spellName, int damage, int mana, int spellRange, string target) 
    : name(spellName), damage(damage), manaCost(mana), range(spellRange), targetType(target) {
    if (targetType == "enemy") 
    {
        description = "Deals " + to_string(damage) + " damage to enemy. Range: " + to_string(range);
    } 
    else
    {
        description = "Deals " + to_string(damage) + " damage to tower. Range: " + to_string(range);
    } 
}

string DirectDamageSpell::GetName() const 
{
    return name;
}

string DirectDamageSpell::GetDescription() const 
{
    return description; 
}

int DirectDamageSpell::GetManaCost() const 
{
    return manaCost;
}

bool DirectDamageSpell::CanCast(const Player& caster, Enemy& enemy) const 
{
    if (targetType == "tower") 
    {
        return false;
    }
    return caster.GetMana() >= manaCost && caster.HasEnemyInRange(range, enemy);
}

void DirectDamageSpell::Cast(Player& caster, Enemy& enemy) 
{
    if (targetType == "tower") 
    {
        cout << "This spell can only target towers!" << endl;
        return;
    }
    
    cout << caster.GetNickname_player() << " casts " << name << "!" << endl;
    caster.DealDamage_player(damage, enemy);
    caster.UseMana(manaCost);
    cout << "Dealt " << damage << " damage to " << enemy.GetNickname_enemy() << "!" << endl;
}

int DirectDamageSpell::GetDamage() const 
{
    return damage;
}
