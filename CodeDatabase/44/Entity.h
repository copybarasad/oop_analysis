#pragma once
#include <string>

class Entity {
public:
    Entity(int health, int damage, int x, int y);
    virtual ~Entity() = default;
    
    int GetBaseHealth() const;
    int GetBaseDamage() const;
    int GetHealth() const;
    int GetDamage() const;
    int GetX() const;
    int GetY() const;
    int GetPrevX() const;
    int GetPrevY() const;
    virtual std::string GetDescription() const = 0;
    bool GetWasOnSlow() const;

    void SetSlowed(bool value);
    bool IsSlowed() const;
    void SetHealth(int health);
    void SetDamage(int damage);
    void SetWasOnSlow(bool value);
    void SetPosition(int new_x, int new_y);
    void SetPrevPosition(int prev_x, int prev_y);
    void SetBaseHealth(int health);
    void SetBaseDamage(int damage);

    void TakeDamage(int damage);
    virtual void Attack(Entity& target) = 0;

    virtual void TakeSpellDamage(int damage, bool can_slow = false) = 0;
protected:
    int base_health_;
    int base_damage_;
    int health_;
    int damage_;
    int x_;
    int y_;
    int prev_x_;
    int prev_y_;
    bool was_on_slow_;
    bool slowed_;
};
