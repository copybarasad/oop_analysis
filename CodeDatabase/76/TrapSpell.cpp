#include "TrapSpell.h"
#include "Player.h"
#include <iostream>

using namespace std;

TrapSpell::TrapSpell(string spellName, int damage, int mana) 
    : name(spellName), damage(damage), manaCost(mana){
    description = "Places a trap that deals " + to_string(damage) + " damage when enemy steps on it.";
}

string TrapSpell::GetName() const 
{
    return name;
}

string TrapSpell::GetDescription() const 
{
    return description;
}

int TrapSpell::GetManaCost() const 
{
    return manaCost;
}

int TrapSpell::GetDamage() const 
{
    return damage;
}

bool TrapSpell::CanCast(const Player& caster, Enemy& enemy) const 
{
    return caster.GetMana() >= manaCost;
}

void TrapSpell::Cast(Player& caster, Enemy& enemy) 
{
    if (!CanCast(caster, enemy)) 
    {
        cout << "Not enough mana to cast " << name << "!" << endl;
        return;
    }
    
    auto field = caster.GetField();
    if (!field) 
    {
        cout << "Cannot cast trap - field not set!" << endl;
        return;
    }
    
    int trapX, trapY;
    cout << "Enter coordinates to place " << name << " (x y): ";
    cin >> trapX >> trapY;
    
    if (!field->IsValidPosition(trapX, trapY)) 
    {
        cout << "Invalid position for trap! Position must be within field boundaries and not on a wall." << endl;
        return;
    }
    
    if (!field->IsCellFree(trapX, trapY)) 
    {
        cout << "Cell is occupied! Cannot place trap here." << endl;
        return;
    }
    
    field->SetCellSymbol(trapX, trapY, 'T', true); 
    
    caster.UseMana(manaCost);
    
    cout << caster.GetNickname_player() << " placed " << name << " at (" << trapX << ", " << trapY << ")" << endl;
}

void TrapSpell::CheckTraps(PlayingField& field, Enemy& enemy) 
{
    if (!enemy.IsAlive_enemy()) return;
    
    int enemyX = enemy.GetX_enemy();
    int enemyY = enemy.GetY_enemy();
    
    const auto& grid = field.GetGrid();
    
    if (enemyX >= 0 && enemyX < field.GetWidth() && enemyY >= 0 && enemyY < field.GetHeight()) 
    {
        if (grid[enemyY][enemyX].GetSymbol() == 'T') 
        {
            cout << enemy.GetNickname_enemy() << " triggered a trap!" << endl;
            
            enemy.TakeDamage_enemy(15); 
            
            field.FreeCell(enemyX, enemyY);
            cout << "Trap disappeared after being triggered." << endl;
        }
    }
}