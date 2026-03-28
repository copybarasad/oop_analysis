#pragma once
#include <stdexcept>

class Game;

class Ally {
public:
    Ally(int max_health, int damage)
        : max_health_(max_health),
          health_(max_health),
          damage_(damage) {
        if (max_health_ <= 0 || damage_ <= 0) {
            throw std::logic_error("Ally parameters must be > 0");
        }
    }


    int GetHealth() const { return health_; }
    int GetDamage() const { return damage_; }
    bool IsAlive() const { return health_ > 0; }

    void Move(Game &game, std::size_t x, std::size_t y);

    void TakeDamage(int amount) {
        if (amount < 0) return;
        health_ -= amount;
        if (health_ < 0) health_ = 0;
    }

private:
    int max_health_;
    int health_;
    int damage_;
};
