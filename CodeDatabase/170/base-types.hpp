#ifndef BASE_TYPES_HPP
#define BASE_TYPES_HPP

#include <cstddef>
#include <utility>
#include <limits>

namespace game
{
  using Position = std::pair< size_t, size_t >;

  constexpr size_t INITIAL_COORD_VALUE = std::numeric_limits< size_t >::max();

  enum class SquareType
  {
    Player,
    Enemy,
    EnemyTower,
    Trap,
    Empty
  };

  struct Square
  {
    SquareType type = SquareType::Empty;
    size_t trap_damage = 0;
  };
}

#endif
