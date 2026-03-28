#ifndef CHARACTER_H
#define CHARACTER_H

class Character {
public:
    Character(int health, int damage);
    virtual ~Character() = default;

    int Health() const;
    int MaxHealth() const;
    int Damage() const;
    bool IsAlive() const;
    void TakeDamage(int amount);
    void SetMaxHealth(int max_health);
    void SetHealth(int health);
    void SetDamage(int damage);
    void IncreaseDamage(int amount);

private:
    int health_;
    int max_health_;
    int damage_;
};

#endif
