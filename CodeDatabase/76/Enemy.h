#pragma once
#include <string>

using namespace std;

class PlayingField;
class Player;

class Enemy
{
private:
    int hp_enemy;
    int maxhp_enemy;
    int damage_enemy;
    string nickname_enemy;
    int x_enemy;
    int y_enemy;
    const PlayingField* field;

    void SetPosition_enemy(int x_enemy, int y_enemy);
    int GenerateRandomDamage_enemy();
    int GenerateRandomCoordinate_enemy();
    string GenerateRandomName_enemy();

public:
    Enemy();

    void RandomSpawn();
    
    int GetDamage_enemy() const;
    int GetHp_enemy() const;
    int GetMaxHp_enemy() const;
    string GetNickname_enemy() const;
    int GetX_enemy() const;
    int GetY_enemy() const;
    void SetField(const PlayingField* field);
    void SetMaxHp_enemy(int maxhp); 
    
    bool IsAlive_enemy() const;
    void Statistics_enemy() const;
    void TakeDamage_enemy(int damage);
    void AI_MakeMove(Player &player);
    bool CanAttackPlayer(const Player& player) const;
    void AttackPlayer(Player& player);

    void SetNickname_enemy(const string& name);
    void SetX_enemy(int x);
    void SetY_enemy(int y);
    void SetHp_enemy(int hp);  
    void SetDamage_enemy(int damage);
};
