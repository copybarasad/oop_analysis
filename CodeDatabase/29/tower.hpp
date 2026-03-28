#ifndef TOWER_HPP
#define TOWER_HPP
#include "actor.hpp"
class EnemyTower : public Actor {
    int range_;
    int cooldown_;
public:
    EnemyTower(int hp,int dmg, Point pos,int range);
    int Range() const noexcept;
    bool OnCooldown() const noexcept;
    void Tick();
    void SetCooldown(int cd);
    int Cooldown() const noexcept;
};
#endif
