#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
    Enemy(int x, int y, int health, int damage);

    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDamage() const;
    int GetX() const;
    int GetY() const;
    int GetScoreValue() const;

    void TakeDamage(int damage);
    void SetPosition(int x, int y);
    bool IsAlive() const;
    void IncreaseStats(int health_bonus, int damage_bonus);

private:
    int health_;
    int max_health_;
    int damage_;
    int x_;
    int y_;
    int score_value_;
};

#endif // ENEMY_H
