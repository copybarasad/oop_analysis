#ifndef ALLY_HPP
#define ALLY_HPP

#include "hero.hpp"
#include "enemy.hpp"
#include "field.hpp"
#include <vector>

class Ally : public Hero {
private:
    int x_;
    int y_;
    int attack_count_ = 0;
    int max_attacks_ = 3;
public:
    Ally(const std::string& name, int health, int damage, int x, int y);

    int GetX() const { return x_; }
    int GetY() const { return y_; }

    void SetPosition(int x, int y);
    void Move(std::vector<Enemy>& enemies, Field& field, const Player& player);

    bool IsExpired() const { return attack_count_ >= max_attacks_; }
    void IncreaseAttackCount() { attack_count_++; }
};

#endif