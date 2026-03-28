#ifndef ALLY_HPP_INCLUDED
#define ALLY_HPP_INCLUDED

#include "position.hpp"
#include "enemy.hpp"
#include <vector>
#include <cmath>

class Ally {
public:
public:
    Ally() = default;
    Ally(Position p, int hp, int dmg): pos_(p), hp_(hp), dmg_(dmg) {}
    const Position& getPos() const noexcept { return pos_; }
    void setPos(const Position& p) noexcept { pos_ = p; }
    int getHp() const noexcept { return hp_; }
    void setHp(int v) noexcept { hp_ = v; }
    int getDmg() const noexcept { return dmg_; }
    void setDmg(int v) noexcept { dmg_ = v; }
public:
    // methods remain public
    Position pos_;
    int hp_{6};
    int dmg_{2};

    static inline int manh(const Position& a, const Position& b) {
        return std::abs(a.getX() - b.getX()) + std::abs(a.getY() - b.getY());
    }

    bool isDead() const noexcept { return hp_ <= 0; }
    void takeDamage(int d) { hp_ -= d; if (hp_ < 0) hp_ = 0; }

    int closestEnemyIndex(const std::vector<Enemy>& enemies) const {
        int best = -1, bestD = 1e9;
        for (int i=0;i<(int)enemies.size();++i) {
            if (enemies[i].isDead()) continue;
            int d = manh(pos_, enemies[i].getPosition());
            if (d < bestD) { bestD = d; best = i; }
        }
        return best;
    }

    static Position stepToward(const Position& from, const Position& to) {
        Position s{0,0};
        if (from.getX() < to.getX()) s.setX(1); else if (from.getX() > to.getX()) s.setX(-1);
        if (from.getY() < to.getY()) s.setY(1); else if (from.getY() > to.getY()) s.setY(-1);
        if (s.getX() && s.getY()) s.setY(0);
        return s;
    }
};

#endif
