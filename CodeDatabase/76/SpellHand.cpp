#include "SpellHand.h"
#include "Player.h"
#include "Enemy.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include <iostream>
#include <ctime>
#include <random>

using namespace std;

SpellHand::SpellHand(size_t maxSpells) : maxSize(maxSpells), enemiesKilled(0) 
{
    rng.seed(time(0));

    AddRandomStarterSpell();
}

shared_ptr<SpellCard> SpellHand::CreateRandomStarterSpell() 
{
    uniform_int_distribution<int> spellType(0, 2);
    
    if (spellType(rng) == 0) 
    {
        return make_shared<DirectDamageSpell>();
    } 
    else if (spellType(rng) == 1)
    {
        return make_shared<AreaDamageSpell>();
    }
    else
    {
        return make_shared<TrapSpell>();
    }
}

void SpellHand::AddRandomStarterSpell() 
{
    auto starterSpell = CreateRandomStarterSpell();
    AddSpell(starterSpell);
}

bool SpellHand::AddSpell(shared_ptr<SpellCard> spell) 
{
    if (IsFull()) 
    {
        cout << "Spell hand is full! Cannot add more spells." << endl;
        return false;
    }
    spells.push_back(spell);
    return true;
}


bool SpellHand::CanCastSpell(int index, const Player& caster, Enemy& enemy) const 
{
    if (index < 0 || index >= spells.size()) return false;
    return spells[index]->CanCast(caster, enemy); 
}

void SpellHand::CastSpell(int index, Player& caster, Enemy& enemy) 
{
    if (index < 0 || index >= spells.size()) 
    {
        cout << "Invalid spell index!" << endl;
        return;
    }
    
    if (spells[index]->CanCast(caster, enemy)) 
    { 
        string spellName = spells[index]->GetName();
        
        spells[index]->Cast(caster, enemy);       
        cout << caster.GetNickname_player() << " spent a turn casting spell." << endl;
        
        spells.erase(spells.begin() + index);
        cout << spellName << " disappeared from spell hand!" << endl;
    } 
    else 
    {
        cout << "Cannot cast " << spells[index]->GetName() << "!" << endl;
    }
}

void SpellHand::DisplaySpells(const Player& player, Enemy& enemy) const 
{ 
    cout << "=== Spell Hand ===" << endl;
    for (size_t i = 0; i < spells.size(); ++i) 
    {
        cout << i << ". " << spells[i]->GetName() 
             << " - " << spells[i]->GetDescription()
             << " (Mana: " << spells[i]->GetManaCost() << ")";
        
        if (!spells[i]->CanCast(player, enemy)) 
        {  
            cout << " [CANNOT CAST]";
        }
        cout << endl;
    }
    cout << "Spells: " << spells.size() << "/" << maxSize << endl;
}

void SpellHand::RemoveSpell(int index) 
{
    if (index >= 0 && index < spells.size()) 
    {
        cout << "Removed spell: " << spells[index]->GetName() << endl;
        spells.erase(spells.begin() + index);
    }
}

size_t SpellHand::GetSpellCount() const 
{
    return spells.size();
}

bool SpellHand::IsFull() const 
{
    return spells.size() >= maxSize;
}

void SpellHand::Clear() 
{
    spells.clear();
    cout << "Spell hand cleared!" << endl;
}

bool SpellHand::BuySpell(shared_ptr<SpellCard> spell, int cost, Player& player) 
{
    if (player.GetMana() < cost) 
    {
        cout << "Not enough mana to buy " << spell->GetName() << "! Need " << cost << ", have " << player.GetMana() << endl;
        return false;
    }
    
    if (IsFull()) 
    {
        cout << "Spell hand is full! Cannot buy " << spell->GetName() << endl;
        return false;
    }
    
    player.SetMana(player.GetMana() - cost);  
    AddSpell(spell);
    cout << "Purchased " << spell->GetName() << " for " << cost << " mana!" << endl;
    return true;
}

void SpellHand::AddRandomSpell() 
{
    if (IsFull()) 
    {
        cout << "Spell hand is full! Cannot add random spell." << endl;
        return;
    }
    
    uniform_int_distribution<int> spellType(0, 2);
    shared_ptr<SpellCard> randomSpell;
    
    if (spellType(rng) == 0) 
    {
        randomSpell = make_shared<DirectDamageSpell>("Fireball", 20, 15, 4);
    } 
    else if (spellType(rng) == 1)
    {
        randomSpell = make_shared<AreaDamageSpell>("Bomb", 12, 20, 3);
    }
    else
    {
        randomSpell = make_shared<TrapSpell>("Bear Trap", 15, 10);   
    }
    
    AddSpell(randomSpell);
    cout << "Found random spell: " << randomSpell->GetName() << endl;
}

shared_ptr<SpellCard> SpellHand::GetSpell(int index) const {
    if (index >= 0 && index < spells.size()) {
        return spells[index];
    }
    return nullptr;
}
