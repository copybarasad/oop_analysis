#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include "position.hpp"
#include <optional>

// простой спавнер
class Spawner {
public:
    Spawner(Position p, int period): pos_(p), period_(period) {}

    // если пришло время и есть свободная соседняя клетка — возвращает позицию спавна
    template <typename IsFreeFunc>
    std::optional<Position> onTurn(IsFreeFunc&& is_free) {
        ++tick_;
        if (tick_ % period_ != 0) return std::nullopt;
        const Position dirs[4] {{1,0},{-1,0},{0,1},{0,-1}};
        for (const auto& d : dirs) {
            Position cand{pos_.getX() + d.getX(), pos_.getY() + d.getY()};
            if (is_free(cand)) return cand;
        }
        return std::nullopt;
    }

    // позиция самого спавнера
    Position getPos() const noexcept { return pos_; }

private:
    // данные спавнера
    Position pos_;
    int period_{5};
    int tick_{0};
};

#endif