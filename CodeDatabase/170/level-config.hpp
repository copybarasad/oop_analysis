#ifndef LEVEL_CONFIG_HPP
#define LEVEL_CONFIG_HPP

#include <vector>
#include "entity-traits.hpp"

namespace game
{
  struct LevelConfig
  {
    size_t current_level_index = 0;

    size_t rows = 0;
    size_t cols = 0;

    PlayerTraits player_traits;
    Position player_pos = { 0, 0 };

    std::vector< std::pair< EnemyTraits, Position > > enemies;
    std::vector< std::pair< EnemyTowerTraits, Position > > towers;
  };
}

#endif
