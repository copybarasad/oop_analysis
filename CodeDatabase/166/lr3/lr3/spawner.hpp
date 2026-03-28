#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include "position.hpp"
#include <optional>

// простой спавнер
class Spawner {
public:
    Spawner(Position pos, int period, int hp): pos_(pos), period_(period), hp_(hp) {}
    Spawner(Position pos, int period) : Spawner(pos, period, 10) {}
    template <typename IsFreeFunc>
    std::optional<Position> onTurn(IsFreeFunc&& is_free) {
        ++tick_;
        if (tick_ % period_ != 0) return std::nullopt;
        static const Position dirs[4] {{1,0},{-1,0},{0,1},{0,-1}};
        for (const auto& d : dirs) {
            Position cand{pos_.getX() + d.getX(), pos_.getY() + d.getY()};
            if (is_free(cand)) return cand;
        }
        return std::nullopt;
    }

    // позиция самого спавнера
    Position getPos() const noexcept { return pos_; }
    int  getHp()        const { return hp_; }
    void takeDamage(int d)  { hp_ = std::max(0, hp_ - std::max(0, d)); }
    bool isDead()    const { return hp_ <= 0; }
    int getPeriod() const noexcept { return period_; }

public:
    bool hasTower() const noexcept { return tower_spawned_; }
    void markTower() noexcept { tower_spawned_ = true; }

private:
    // данные спавнера
    Position pos_;
    int period_{5};
    int tick_{0};
    int hp_{10};
    bool tower_spawned_{false};
};

#endif