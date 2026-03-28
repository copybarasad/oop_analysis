#include "EnemyTower.h"
#include <cmath>
#include <iostream>

using namespace std;

EnemyTower::EnemyTower(int posX, int posY, int hp, int maxHp, int range, int cooldownTurns) 
    : x(posX), y(posY), hp(hp), maxHp(maxHp), attackRange(range), cooldown(0), maxCooldown(cooldownTurns) {
    
    spell = make_shared<DirectDamageSpell>("Tower Bolt", 8, 0, 1);
    name = "Watchtower";
    damage = 15; // ДОБАВЬТЕ ИНИЦИАЛИЗАЦИЮ ПЕРЕМЕННОЙ damage
}

int EnemyTower::GetX() const 
{
    return x;
}

int EnemyTower::GetY() const 
{
    return y;
}

int EnemyTower::GetAttackRange() const 
{
    return attackRange;
}

string EnemyTower::GetName() const 
{
    return name;
}

bool EnemyTower::CanAttack() const 
{
    return cooldown == 0;
}

void EnemyTower::SetPosition(int posX, int posY) 
{
    if (posX < 0 || posY < 0) 
    {
        throw invalid_argument("Tower position cannot be negative");
    }
    x = posX;
    y = posY;
}

void EnemyTower::Update() 
{
    if (cooldown > 0) 
    {
        cooldown--;
    }
}

bool EnemyTower::IsPlayerInRange(const Player& player) const 
{
    if (!player.IsAlive_player()) return false;
    
    int playerX = player.GetX_player();
    int playerY = player.GetY_player();
    
    int distance = abs(playerX - x) + abs(playerY - y);
    return distance <= attackRange;
}

void EnemyTower::AttackPlayer(Player& player) 
{
    if (!CanAttack()) 
    {
        cout << name << " is on cooldown! " << cooldown << " turns remaining." << endl;
        return;
    }
    
    if (!IsPlayerInRange(player)) 
    {
        cout << name << " cannot attack - player out of range!" << endl;
        return;
    }
    
    if (!player.IsAlive_player()) 
    {
        cout << name << " cannot attack dead player!" << endl;
        return;
    }
    
    // ИСПРАВЛЕНО: используем переменную damage вместо spell->GetDamage()
    cout << name << " deals " << damage << " damage to " << player.GetNickname_player() << "!" << endl;
    player.TakeDamage_player(damage);
    
    cooldown = maxCooldown;
    cout << name << " will be on cooldown for " << cooldown << " turns." << endl;
}

void EnemyTower::DisplayInfo() const 
{
    cout << "Position: (" << x << ", " << y << ")" << endl;
    cout << "Attack Range: " << attackRange << endl;
    // ИСПРАВЛЕНО: используем переменную damage вместо spell->GetDamage()
    cout << "Damage: " << damage << endl;
    cout << "HP: " << hp << "/" << maxHp << endl; // ДОБАВЛЕНО отображение HP
    if (cooldown > 0) 
    {
        cout << "Cooldown: " << cooldown << " turns remaining" << endl;
    } 
    else 
    {
        cout << "Status: Ready to attack" << endl;
    }
}

void EnemyTower::TakeDamage(int damage) 
{
    hp -= damage;
    if (hp < 0) hp = 0;
    cout << name << " took " << damage << " damage! HP: " << hp << "/" << maxHp << endl;
}

int EnemyTower::GetHp() const 
{
    return hp;
}

int EnemyTower::GetMaxHp() const 
{
    return maxHp;
}

int EnemyTower::GetDamage() const 
{
    return damage;
}

void EnemyTower::SetHp(int newHp) 
{
    hp = newHp;
    if (hp < 0) hp = 0;
    if (hp > maxHp) hp = maxHp;
}

void EnemyTower::SetMaxHp(int newMaxHp) 
{
    maxHp = newMaxHp;
    if (hp > maxHp) hp = maxHp;
}

void EnemyTower::SetDamage(int newDamage) 
{
    damage = newDamage;
    if (damage < 0) damage = 0;
}