#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

class EnemyTower {
public:
    EnemyTower(int x, int y, int health, int damage, int range = 4, int cooldown = 2);

    int GetX() const;
    int GetY() const;
    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDamage() const;
    int GetRange() const;
    int GetCooldown() const;
    int GetCurrentCooldown() const;

    bool CanAttack() const;
    void Attack();
    void UpdateCooldown();
    void SetCurrentCooldown(int cooldown);

    void TakeDamage(int damage);
    bool IsAlive() const;
    void IncreaseStats(int health_bonus, int damage_bonus);

private:
    int x_;
    int y_;
    int health_;
    int max_health_;
    int damage_;
    int range_;
    int cooldown_;
    int current_cooldown_;
};

#endif // ENEMY_TOWER_H
