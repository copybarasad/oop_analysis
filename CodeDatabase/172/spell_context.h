#pragma once

#include <optional>
#include <random>

#include "pos.h"

namespace game {
    class Field;
    class Player;
    class GameConfig;

    struct SpellContext {
        Field&          field;
        Player&         player;
        GameConfig&     config;
        std::mt19937&   rng;


        std::optional<Pos> targetPos;           // Target cell position (not entity)
        std::optional<Pos> targetEntityPos;     // Target entity position
    };
}