#include "AreaDamageSpell.h"
#include "Player.h"
#include "PlayingField.h"  
#include "EnemyTower.h"  
#include <iostream>

using namespace std;

AreaDamageSpell::AreaDamageSpell(string spellName, int dmg, int mana, int rng) 
: name(spellName), damage(dmg), manaCost(mana), range(rng) {}

string AreaDamageSpell::GetName() const 
{
    return name;
}

string AreaDamageSpell::GetDescription() const 
{
    return "Deals " + to_string(damage) + " damage in " + to_string(range) + "radius";
}

int AreaDamageSpell::GetManaCost() const 
{
    return manaCost;
}

bool AreaDamageSpell::CanCast(const Player& caster, Enemy& enemy) const 
{
    return caster.GetMana() >= manaCost; 
}

void AreaDamageSpell::Cast(Player& caster, Enemy& enemy) 
{
    if (!CanCast(caster, enemy)) 
    {
        cout << "Not enough mana to cast " << name << "!" << endl;
        return;
    }
    
    auto field = caster.GetField();
    if (!field) 
    {
        cout << "Cannot cast spell - field not set!" << endl;
        return;
    }
    
    cout << caster.GetNickname_player() << " casts " << name << "!" << endl;
    
    int playerX = caster.GetX_player();
    int playerY = caster.GetY_player();
    
    int enemyX = enemy.GetX_enemy();
    int enemyY = enemy.GetY_enemy();
    
    if (abs(enemyX - playerX) <= range && abs(enemyY - playerY) <= range) 
    {
        enemy.TakeDamage_enemy(damage);
        cout << "Dealt " << damage << " damage to " << enemy.GetNickname_enemy() << "!" << endl;
    }
    else 
    {
        cout << enemy.GetNickname_enemy() << " is out of range!" << endl;
    }
    
    const_cast<PlayingField*>(field)->DamageTowersInRange(playerX, playerY, range, damage);
    
    caster.UseMana(manaCost);
}