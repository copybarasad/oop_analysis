#ifndef ENEMY_H
#define ENEMY_H

class Player;

class Enemy
{
private:
    int maxhealth;
    int health;
    int damage;
public:
    Enemy();
    int GetHealth() const;
    int GetDamage() const;
    void Damage(int PlayerDanage);
    bool IsLife() const;
    bool EnemyAttack(Player& player);
};

#endif