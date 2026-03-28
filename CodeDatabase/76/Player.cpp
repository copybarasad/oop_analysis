#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "PlayingField.h"
#include "Player.h"
#include "Enemy.h"

using namespace std;

int Player::GenerateRandomDamage_player() 
{
    return rand() % 11 + 5; 
}

void Player::SetField(PlayingField* field) 
{
    if (field == nullptr) 
    {
        throw invalid_argument("Field pointer cannot be null");
    }
    this->field = field;
}

Player::Player(int level, int damage_player, int hp_player, string nickname_player, int x_player, int y_player, int battle, int maxdamage)
:level(level), damage_player(damage_player), hp_player(hp_player), nickname_player(nickname_player), x_player(x_player), y_player(y_player), battle(battle)
{
    if (level < 1) {
        throw invalid_argument("Level must be at least 1. Received: " + to_string(level));
    }
    if (hp_player < 0) {
        throw invalid_argument("Health cannot be negative. Received: " + to_string(hp_player));
    }
    if (x_player < 0) {
        throw invalid_argument("X position cannot be negative. Received: " + to_string(x_player));
    }
    if (y_player < 0) {
        throw invalid_argument("Y position cannot be negative. Received: " + to_string(y_player));
    }
    if (battle != 0 && battle != 1) {
        throw invalid_argument("Battle type must be 0 or 1. Received: " + to_string(battle));
    }

    this->maxhp_player = 100;
    this->mana = 100;
    this->maxbattle = battle*2;
    if (damage_player == -1) {
        this->damage_player = GenerateRandomDamage_player();
    } 
    else 
    {
        if (damage_player < 0) 
        {
            throw invalid_argument("Damage cannot be negative. Received: " + to_string(damage_player));
        }
        this->damage_player = damage_player;
    }
    
    if (battle == 0) {
        this->damage_player = this->damage_player/2;
    }
    
    this->maxdamage = this->damage_player*2;
}

int Player::GetLevel() const 
{ 
    if (level < 1) 
    {
        throw logic_error("Player level is in invalid state");
    }
    return level; 
}

int Player::GetMana() const 
{ 
    return mana; 
}

int Player::GetDamage_player() const 
{ 
    if (damage_player < 0) 
    {
        throw logic_error("Player damage is in invalid state");
    }
    return damage_player; 
}

int Player::GetHp_player() const 
{ 
    if (hp_player < 0) 
    {
        throw logic_error("Player health is in invalid state");
    }
    return hp_player; 
}

int Player::GetMaxHp_player() const 
{ 
    if (maxhp_player <= 0) 
    {
        throw logic_error("Player max health is in invalid state");
    }
    return maxhp_player; 
}

int Player::GetMaxDamage() const 
{ 
    if (maxdamage < 0) 
    {
        throw logic_error("Player max damage is in invalid state");
    }
    return maxdamage; 
}

int Player::GetBattle() const 
{ 
    if (battle != 0 && battle != 1) 
    {
        throw logic_error("Player battle type is in invalid state");
    }
    return battle; 
}

int Player::GetMaxBattle() const 
{ 
    return maxbattle; 
}

string Player::GetNickname_player() const 
{ 
    if (nickname_player.empty()) 
    {
        throw logic_error("Player nickname is empty");
    }
    return nickname_player; 
}

int Player::GetX_player() const 
{ 
    if (x_player < 0) {
        throw logic_error("Player X position is in invalid state");
    }
    return x_player; 
}

int Player::GetY_player() const 
{ 
    if (y_player < 0) {
        throw logic_error("Player Y position is in invalid state");
    }
    return y_player; 
}

bool Player::IsAlive_player() const { return hp_player>0;}

void Player::MoveUp() 
{ 
    if (!field) 
    {
        throw logic_error("Cannot move - field is not set");
    }
    if (y_player > 1 && field->IsValidPosition(x_player, y_player-1) && field->IsCellFree(x_player, y_player-1)) 
    {
        y_player--;
    }
}

void Player::MoveDown() 
{ 
    if (!field) 
    {
        throw logic_error("Cannot move - field is not set");
    }
    if (y_player < field->GetHeight() - 2 && field->IsValidPosition(x_player, y_player+1) && field->IsCellFree(x_player, y_player+1)) 
    {
        y_player++;
    }
}

void Player::MoveLeft() 
{ 
    if (!field) 
    {
        throw logic_error("Cannot move - field is not set");
    }
    if (x_player > 1 && field->IsValidPosition(x_player-1, y_player) && field->IsCellFree(x_player-1, y_player)) 
    {
        x_player--;
    }
}

void Player::MoveRight() 
{ 
    if (!field) 
    {
        throw logic_error("Cannot move - field is not set");
    }
    if (x_player < field->GetWidth() - 2 && field->IsValidPosition(x_player+1, y_player) && field->IsCellFree(x_player+1, y_player)) 
    {
        x_player++;
    }
}

void Player::Statistics_player() const {
    cout << "=== " << nickname_player << " ===" << endl;
    cout << "HP: " << hp_player << "/" << maxhp_player << endl;
    cout << "Level: " << level << endl;
    cout << "Damage: " << damage_player << "/" << maxdamage <<endl;
    cout << "Battle: " << battle << endl;
    cout << "Position: (" << x_player << ", " << y_player << ")" << endl;
    cout << "Mana: " << mana << endl;
}

void Player::Instruction() const {
    cout << "W - move up" << endl;
    cout << "S - move down" << endl;
    cout << "D - move right" << endl;
    cout << "A - move left" << endl;
    cout << "E - exit" << endl;
    cout << "I - instruction manual" << endl;
    cout << "Z - statistics" << endl;
    cout << "B - changing the damage range" << endl;
    cout << "C - cast spell" << endl;
    cout << "H - show spells" << endl;
    cout << "P - purchase spells" << endl;
    cout << "S - save game" << endl;
    cout << "L - load game" << endl;
}

void Player::SetLevel(int level)
{
    if (level < 1) 
    {
        throw invalid_argument("Level must be at least 1. Received: " + to_string(level));
    }
    this->level=level;
}

void Player::SetMana(int newMana)
{
    if (newMana < 0) 
    {
        this->mana = 0;
    } 
    else if (newMana > 100) 
    {
        this->mana = 100;
    } 
    else 
    {
        this->mana = newMana;
    }
}

void Player::SetBattle(int battle)
{
    int oldBattle = this->battle;
    this->battle=battle;

    if (oldBattle != battle) {
        if (battle == 0) {
            damage_player = maxdamage / 2;
            cout << "Switched to close combat. Damage: " << damage_player << endl;
        } 
        else 
        {
            damage_player = maxdamage;
            cout << "Switched to long-range combat. Damage: " << damage_player << endl;
        }
    }
}

void Player::SetDamage_player(int damage_player)
{
    if (damage_player < 0) 
    {
        throw invalid_argument("Damage cannot be negative. Received: " + to_string(damage_player));
    }
    this->damage_player=damage_player;
}

void Player::SetHp_player(int hp_player)
{
    if (hp_player < 0) 
    {
        throw invalid_argument("Health cannot be negative. Received: " + to_string(hp_player));
    }
    this->hp_player=hp_player;
    if (this->hp_player>maxhp_player)
    {
        this->hp_player=maxhp_player;
    }
}

void Player::SetNickname_player(string nickname_player)
{
    if (nickname_player.empty()) 
    {
        throw invalid_argument("Nickname cannot be empty");
    }
    this->nickname_player=nickname_player;
}

void Player::SetX_player(int x_player)
{
    if (x_player < 0) 
    {
        throw invalid_argument("X position cannot be negative. Received: " + to_string(x_player));
    }
    this->x_player=x_player;
}

void Player::SetY_player(int y_player)
{
    if (y_player < 0) 
    {
        throw invalid_argument("Y position cannot be negative. Received: " + to_string(y_player));
    }
    this->y_player=y_player;
}

void Player::TakeDamage_player(int damage_player) 
{ 
    if (damage_player < 0) 
    {
        throw invalid_argument("Damage taken cannot be negative. Received: " + to_string(damage_player));
    }
    hp_player -= damage_player; 
    mana-=10;
    if (hp_player < 0) hp_player = 0;
}

void Player::DealDamage_player(int damage_player, Enemy &enemy) 
{  
    if (damage_player < 0) 
    {
        throw invalid_argument("Damage dealt cannot be negative. Received: " + to_string(damage_player));
    }
    if (!enemy.IsAlive_enemy()) 
    {
        throw logic_error("Cannot attack dead enemy");
    }

    int deltaX = abs(enemy.GetX_enemy() - this->GetX_player());
    int deltaY = abs(enemy.GetY_enemy() - this->GetY_player());
    bool attackSuccessful = false;
    
    if (battle == 0 && deltaX <= 1 && deltaY <= 1 && (deltaX + deltaY > 0))   
    {
        enemy.TakeDamage_enemy(damage_player); 
        cout << GetNickname_player() << " attacks " << enemy.GetNickname_enemy() << " for " << damage_player << " damage!" << endl;
        attackSuccessful = true;
        mana+=5;
    }
    else if (battle == 1 && deltaX <= 2 && deltaY <= 2 && (deltaX + deltaY > 0))   
    {
        enemy.TakeDamage_enemy(damage_player); 
        cout << GetNickname_player() << " attacks " << enemy.GetNickname_enemy() << " for " << damage_player << " damage!" << endl;
        attackSuccessful = true;
        mana+=5;
    }
}

bool Player::CanMakeMove() const 
{
    if (!field) 
    {
        return false;
    }
    
    return (y_player > 1 && field->IsValidPosition(x_player, y_player-1) && field->IsCellFree(x_player, y_player-1)) ||  // Up
           (y_player < field->GetHeight() - 2 && field->IsValidPosition(x_player, y_player+1) && field->IsCellFree(x_player, y_player+1)) ||  // Down
           (x_player > 1 && field->IsValidPosition(x_player-1, y_player) && field->IsCellFree(x_player-1, y_player)) ||  // Left
           (x_player < field->GetWidth() - 2 && field->IsValidPosition(x_player+1, y_player) && field->IsCellFree(x_player+1, y_player));  // Right
}

void Player::UseMana(int amount) 
{
    if (amount < 0) {
        throw invalid_argument("Mana usage amount cannot be negative. Received: " + to_string(amount));
    }
    if (mana < amount) {
        throw logic_error("Not enough mana to use. Required: " + to_string(amount) + ", Available: " + to_string(mana));
    }
    
    mana -= amount;
    cout << "Used " << amount << " mana. Remaining: " << mana << "/100" << endl;
}

const PlayingField* Player::GetField() const
{
        return field;
}

PlayingField* Player::GetField()
{
        return field;
}

bool Player::HasEnemyInRange(int range, const Enemy& enemy) const 
{
    if (!enemy.IsAlive_enemy()) {
        return false;
    }
    
    int deltaX = abs(enemy.GetX_enemy() - x_player);
    int deltaY = abs(enemy.GetY_enemy() - y_player);

    return (deltaX <= range && deltaY <= range);
}

