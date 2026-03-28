#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
    Enemy(int max_health, int damage);

    int GetHealth() const;
    int GetDamage() const;
    bool IsAlive() const;

    void TakeDamage(int amount);

private:
    int max_health_;
    int health_;
    int damage_;

    void EnsureInvariants() const;
};

#endif