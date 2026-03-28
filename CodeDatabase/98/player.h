#ifndef PLAYER_H
#define PLAYER_H

#include "AttackType.h"

class Enemy;

class Player
{
private:
    int speed;
    int maxhealth;
    int health;
    int meleeDamage;
    int rangedDamage; 
    int countmurder; //это типа и буджут как очки 
    AttackType currentAttackType;
public:
    Player();
    //узнать статы
    int GetHealth() const;
    int GetMaxHealth() const;
    int GetSpeed() const;
    int GetMurder() const;
    int GetDamage() const;
    void NewMurder();
    //для боя
    bool PlayerAttack(Enemy& enemy);
    void Damage(int EnemyDamage);
    bool IsLife() const;
    void SwitchAttackType();
    int GetMeleeDamage() const;
    int GetRangedDamage() const;
    AttackType GetAttackType() const;
    //прочее
    void Update(int count);
    void RestoreHealth();
    void UpdateMaxHealth(int item);
    void UpdateMeleeDamage(int item);
    void UpdateRangedDamage(int item);
    void UpdateSpeed(int aitem);

};

#endif 