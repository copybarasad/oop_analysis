#pragma once
#include <string>
#include <algorithm>

class GameField;
class Renderer;

class Entity {
protected:
    std::string name_;
    int health_;
    int damage_;
public:
    Entity(std::string name, int hp, int dmg)
        : name_(std::move(name)), health_(hp), damage_(dmg) {}

    virtual ~Entity() = default;

    const std::string& Name() const noexcept { return name_; }
    int Health() const noexcept { return health_; }
    int Damage() const noexcept { return damage_; }
    bool IsAlive() const noexcept { return health_ > 0; }

    void TakeDamage(int dmg) noexcept {
        if (dmg > 0) health_ = std::max(0, health_ - dmg);
    }

    virtual void Update(GameField& field) = 0;
};
