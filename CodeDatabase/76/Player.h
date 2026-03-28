#pragma once
#include <string>

using namespace std;

class PlayingField;
class Enemy;

class Player
{
private:
    int level; 
    int damage_player; 
    int hp_player; 
    int maxhp_player=100;
    int x_player; 
    int y_player; 
    int battle;
    int maxbattle;
    int maxdamage;
    int mana=100;
    string nickname_player;
    PlayingField* field;

    int GenerateRandomDamage_player();

public:
    Player(int level = 1, int damage_player = -1, int hp_player = 100, string nickname_player = "Player1", int x_player = 1, int y_player = 1, int battle=0, int maxdamage=15);

    int GetLevel() const;
    int GetMana() const;
    int GetDamage_player() const;  
    int GetHp_player() const;
    int GetMaxDamage() const;
    int GetMaxHp_player() const;
    int GetMaxBattle() const;
    int GetBattle() const;
    string GetNickname_player() const;
    int GetX_player() const;
    int GetY_player() const;
    const PlayingField* GetField() const;
    PlayingField* GetField();

    void SetLevel(int level);
    void SetMana(int mana);
    void SetDamage_player(int damage_player);
    void SetHp_player(int hp_player);
    void SetBattle(int battle);
    void SetNickname_player(string nickname_player);
    void SetX_player(int x_player);
    void SetY_player(int y_player);
    void SetField(PlayingField* field);

    bool IsAlive_player() const;

    void MoveDown();
    void MoveUp();
    void MoveLeft();
    void MoveRight();

    void TakeDamage_player(int damage_player);
    void DealDamage_player(int damage_player, Enemy &enemy);

    void Statistics_player() const;

    void Instruction() const;
    bool CanMakeMove() const;

    void UseMana(int amount);

    bool HasEnemyInRange(int range, const Enemy& enemy) const;
};
