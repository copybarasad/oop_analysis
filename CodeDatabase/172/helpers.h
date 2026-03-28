#pragma once

#include <optional>

#include "pos.h"
#include "field.h"
#include "enemy.h"

namespace game::helpers {
    inline int manhattan(const game::Pos a, const game::Pos b) {
        game::Pos f = a;
        game::Pos s = b;

        return std::abs(f.x - s.x) + std::abs(f.y - s.y);
    }
}