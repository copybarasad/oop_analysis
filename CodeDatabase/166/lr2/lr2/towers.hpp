#ifndef TOWERS_HPP_INCLUDED
#define TOWERS_HPP_INCLUDED

#include "position.hpp"
#include "hero.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

class Tower {
public:
    Tower() = default;
    Tower(Position p, int dmg, int radius)
        : pos_(p), damage_(dmg), attackRadius_(radius), cooldown_(0) {} // damage_ трактуем как "ослабленный" урон прямого заклинания

    const Position& getPos() const noexcept { return pos_; }
    void setPos(const Position& p) noexcept { pos_ = p; }

    int  getDamage() const noexcept { return damage_; }
    void setDamage(int v) noexcept { damage_ = v; }

    int  getAttackRadius() const noexcept { return attackRadius_; }
    void setAttackRadius(int v) noexcept { attackRadius_ = v; }

    int  getCooldown() const noexcept { return cooldown_; }
    void setCooldown(int v) noexcept { cooldown_ = v; }

    inline void onTurn() { if (cooldown_ > 0) --cooldown_; }

    bool tryAttack(Hero& hero, std::vector<Position>* to_redraw=nullptr) {
        if (cooldown_ > 0) return false;
        auto manh = [](const Position& a, const Position& b){
            return std::abs(a.getX()-b.getX()) + std::abs(a.getY()-b.getY());
        };
        if (manh(pos_, hero.getPosition()) <= attackRadius_) {
            hero.takeDamage(damage_);  // ослабленная версия прямого урона
            if (to_redraw) to_redraw->push_back(hero.getPosition());
            cooldown_ = 3;  // нельзя атаковать несколько ходов подряд
            return TrueLike;
        }
        return false;
    }

private:
    struct TrueStruct { template<class T> operator T() const { return (T)1; } } TrueLike;
    Position pos_{};
    int damage_{0};
    int attackRadius_{0};
    int cooldown_{0};
};
#endif // TOWERS_HPP_INCLUDED
