#pragma once
#include "entity.hpp"
#include "hand.hpp"

class Enemy;
class Player : public Entity {
public:
    Player(int max_health, int base_damage);

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getKills() const noexcept { return kills_; }
    int getMaxHealth() const noexcept { return max_health_; }
    int getScore() const noexcept { return score_; }
    bool isRanged() const noexcept { return ranged_mode_; }
    int getDamage() const noexcept { return damage_; }
    void addDamage(int value) noexcept { damage_ += value; }

    void addMaxHealth(int value) noexcept { max_health_ += value; }
    bool isSlowed() const noexcept { return slowed_; }
    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
    void setSlowed(bool value) noexcept { slowed_ = value; }
    
    bool SpendScore(int amount);
    void Attack(Entity& target) override;
    int getEffectiveDamage() const noexcept override;
    void ToggleMode();
    void Heal(int amount);
    void AddScore(int delta);
    void AddKills();
    Hand& getHand() noexcept { return hand_; }
    const Hand& getHand() const noexcept { return hand_; }

    void setHealth(int h) noexcept { health_ = h; }
    void setMaxHealth(int h) noexcept { max_health_ = h; }
    void setDamage(int d) noexcept { damage_ = d; }
    void setScore(int s) noexcept { score_ = s; }
    void setKills(int k) noexcept { kills_ = k; }
    void setRanged(bool r) noexcept { ranged_mode_ = r; }

private:
    int x_;
    int y_;
    int max_health_;
    int score_;
    bool slowed_;
    bool ranged_mode_; 
    int kills_;
    Hand hand_;
};
