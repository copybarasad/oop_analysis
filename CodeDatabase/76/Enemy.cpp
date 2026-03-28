#include "Enemy.h"
#include "Player.h"
#include "PlayingField.h"
#include "TrapSpell.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <utility>
#include <queue>

using namespace std;

void Enemy::SetPosition_enemy(int x_enemy, int y_enemy)
{
    if (x_enemy < 0) 
    {
        throw invalid_argument("Enemy X position cannot be negative. Received: " + to_string(x_enemy));
    }
    if (y_enemy < 0) 
    {
        throw invalid_argument("Enemy Y position cannot be negative. Received: " + to_string(y_enemy));
    }
    if (field && !field->IsValidPosition(x_enemy, y_enemy)) 
    {
        throw invalid_argument("Enemy position is outside field boundaries: (" + to_string(x_enemy) + ", " + to_string(y_enemy) + ")");
    }

    this->x_enemy=x_enemy;
    this->y_enemy=y_enemy;
}

void Enemy::SetHp_enemy(int hp_enemy)
{
    if (hp_enemy < 0) 
    {
        throw invalid_argument("Enemy health cannot be negative. Received: " + to_string(hp_enemy));
    }
    if (hp_enemy > maxhp_enemy) 
    {
        throw invalid_argument("Enemy health cannot exceed maximum. Max: " + to_string(maxhp_enemy) + ", Received: " + to_string(hp_enemy));
    }

    this->hp_enemy=hp_enemy;
}

int Enemy::GenerateRandomDamage_enemy() 
{
    return rand() % 11 + 5; 
}

int Enemy::GenerateRandomCoordinate_enemy() 
{
    int maxSize = (field->GetWidth() > field->GetHeight()) ? field->GetHeight() : field->GetWidth();
    return rand() % (maxSize - 2) + 1;
}

string Enemy::GenerateRandomName_enemy() 
{
    string names[] = {"Goblin", "Orc", "Troll", "Skeleton", "Zombie", "Vampire"};
    return names[rand() % 6];
}

Enemy::Enemy() 
{
    hp_enemy = 20;
    maxhp_enemy = hp_enemy;
    damage_enemy = 5;
    nickname_enemy = GenerateRandomName_enemy();
    x_enemy = 1;
    y_enemy = 1;
    field = nullptr;
}

int Enemy::GetDamage_enemy() const 
{ 
    if (damage_enemy < 0) 
    {
        throw logic_error("Enemy damage is in invalid state");
    }
    return damage_enemy; 
}

int Enemy::GetHp_enemy() const 
{ 
    if (hp_enemy < 0) 
    {
        throw logic_error("Enemy health is in invalid state");
    }
    return hp_enemy; 
}

int Enemy::GetMaxHp_enemy() const 
{ 
    if (maxhp_enemy <= 0) 
    {
        throw logic_error("Enemy max health is in invalid state");
    }
    return maxhp_enemy; 
}

string Enemy::GetNickname_enemy() const 
{ 
    if (nickname_enemy.empty()) 
    {
        throw logic_error("Enemy nickname is empty");
    }
    return nickname_enemy; 
}

int Enemy::GetX_enemy() const 
{ 
    if (x_enemy < 0) 
    {
        throw logic_error("Enemy X position is in invalid state");
    }
    return x_enemy; 
}

int Enemy::GetY_enemy() const 
{ 
    if (y_enemy < 0) 
    {
        throw logic_error("Enemy Y position is in invalid state");
    }
    return y_enemy; 
}

void Enemy::SetField(const PlayingField* fieldPtr) {
    field = fieldPtr;
    if (x_enemy == 1 && y_enemy == 1 && field) {
        do {
            x_enemy = GenerateRandomCoordinate_enemy();
            y_enemy = GenerateRandomCoordinate_enemy();
        } while ((x_enemy == 1 && y_enemy == 1) || !field->IsCellFree(x_enemy, y_enemy));
        cout << "DEBUG: Enemy spawned at random position (" << x_enemy << ", " << y_enemy << ")" << endl;
    } else if (field) {
        cout << "DEBUG: Enemy loaded at position (" << x_enemy << ", " << y_enemy << ")" << endl;
    }
}

bool Enemy::IsAlive_enemy() const { return hp_enemy>0;}

void Enemy::Statistics_enemy() const 
{
    if (nickname_enemy.empty()) 
    {
        throw logic_error("Cannot display statistics - enemy nickname is empty");
    }
    cout << "=== " << nickname_enemy << " ===" << endl;
    cout << "HP: " << hp_enemy << "/" << maxhp_enemy << endl;
    cout << "Damage: " << damage_enemy << endl;
    cout << "Position: (" << x_enemy << ", " << y_enemy << ")" << endl;
}

void Enemy::TakeDamage_enemy(int damage_enemy) 
{
    if (damage_enemy < 0) 
    {
        throw invalid_argument("Damage taken cannot be negative. Received: " + to_string(damage_enemy));
    }
    if (!IsAlive_enemy()) 
    {
        throw logic_error("Cannot damage dead enemy");
    }

    hp_enemy -= damage_enemy;
    if (hp_enemy < 0) hp_enemy = 0;
    cout << nickname_enemy << " takes " << damage_enemy << " damage! HP: " << hp_enemy << endl;
}

void Enemy::AI_MakeMove(Player &player) 
{
    if (!field) throw logic_error("Cannot make AI move - field is not set");
    if (!IsAlive_enemy()) throw logic_error("Cannot make AI move - enemy is dead");
    if (!player.IsAlive_player()) throw logic_error("Cannot make AI move - player is dead");
    
    int playerX = player.GetX_player();
    int playerY = player.GetY_player();
    
    vector<pair<int, int>> directions = {
        {1, 0},   // вправо
        {-1, 0},  // влево
        {0, 1},   // вниз
        {0, -1}   // вверх
    };
    
    pair<int, int> best_dir = {0, 0};
    bool found_move = false;
    int min_distance = 1000000; 
    
    for (auto& dir : directions) 
    {
        int newX = x_enemy + dir.first;
        int newY = y_enemy + dir.second;
        
        if (newX >= 1 && newX <= field->GetWidth() - 2 && newY >= 1 && newY <= field->GetHeight() - 2 && 
            field->IsValidPosition(newX, newY) && 
            !field->IsBarrierAt(newX, newY)) 
        {        
            int distance = abs(playerX - newX) + abs(playerY - newY);
            if (!found_move || distance < min_distance) 
            {
                min_distance = distance;
                best_dir = dir;
                found_move = true;
            }
        }
    }
    
    if (found_move) 
    {
        int newX = x_enemy + best_dir.first;
        int newY = y_enemy + best_dir.second;
        
        if (newX == playerX && newY == playerY) 
        {
            AttackPlayer(player);
        } 
        else 
        {
            x_enemy = newX;
            y_enemy = newY;
            cout << endl;
            cout << nickname_enemy << " moves to (" << x_enemy << ", " << y_enemy << ")" << endl;
            
            if (field->HasTrapAt(x_enemy, y_enemy)) 
            {
                cout << nickname_enemy << " stepped on a trap!" << endl;
                TrapSpell::CheckTraps(*const_cast<PlayingField*>(field), *this);
            }
        }
    } 
    else 
    {
        cout << nickname_enemy << " cannot move - all directions blocked" << endl;
    }
}

bool Enemy::CanAttackPlayer(const Player &player) const
{
    if (!IsAlive_enemy()) 
    {
        return false;
    }
    if (!player.IsAlive_player()) 
    {
        return false;
    }

    vector<pair<int, int>> directions = {
        {1, 0},   // вправо
        {-1, 0},  // влево
        {0, 1},   // вниз
        {0, -1}   // вверх
    };
    
    for (auto& dir : directions) 
    {
        int checkX = x_enemy + dir.first;
        int checkY = y_enemy + dir.second;
        
        if (checkX == player.GetX_player() && checkY == player.GetY_player())
        {
            return true;
        }
    }
    return false;
}

void Enemy::AttackPlayer(Player &player)
{
    if (!IsAlive_enemy()) 
    {
        throw logic_error("Dead enemy cannot attack");
    }
    if (!player.IsAlive_player()) 
    {
        throw logic_error("Cannot attack dead player");
    }
    
    if (CanAttackPlayer(player))
    {
        cout << nickname_enemy << " attacks " << player.GetNickname_player() << " for " << damage_enemy << " damage!" << endl;
        player.TakeDamage_player(damage_enemy);
    }
    else
    {
        throw logic_error("Enemy attempted to attack but player is out of range");
    }
}

void Enemy::SetNickname_enemy(const string& name) {
    if (name.empty()) {
        throw invalid_argument("Enemy nickname cannot be empty");
    }
    nickname_enemy = name;
    cout << "Enemy name set to: " << nickname_enemy << endl;
}

void Enemy::SetX_enemy(int x) {
    if (x < 0) {
        throw invalid_argument("Enemy X position cannot be negative");
    }
    x_enemy = x;
    cout << "DEBUG: Enemy X set to " << x_enemy << endl;
}

void Enemy::SetY_enemy(int y) {
    if (y < 0) {
        throw invalid_argument("Enemy Y position cannot be negative");
    }
    y_enemy = y;
    cout << "DEBUG: Enemy Y set to " << y_enemy << endl;
}

void Enemy::SetDamage_enemy(int damage) {
    if (damage < 0) {
        throw invalid_argument("Enemy damage cannot be negative. Received: " + to_string(damage));
    }
    damage_enemy = damage;
    cout << "Enemy damage set to: " << damage_enemy << endl;
}

void Enemy::SetMaxHp_enemy(int maxhp) {
    maxhp_enemy = maxhp;
}

void Enemy::RandomSpawn() {
    if (!field) {
        throw logic_error("Cannot spawn enemy - field is not set");
    }
    
    do {
        x_enemy = GenerateRandomCoordinate_enemy();
        y_enemy = GenerateRandomCoordinate_enemy();
    } while (!field->IsCellFree(x_enemy, y_enemy));
    
    cout << "DEBUG: Enemy randomly spawned at (" << x_enemy << ", " << y_enemy << ")" << endl;
}