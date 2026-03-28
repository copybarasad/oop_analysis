#ifndef ENTITY_TRAITS_HPP
#define ENTITY_TRAITS_HPP

#include "base-types.hpp"

namespace game
{
  constexpr size_t INITIAL_ENTITY_HEALTH_POINTS = 20;
  constexpr size_t INITIAL_ENTITY_ATTACK_DAMAGE = 1;

  constexpr size_t INITIAL_PLAYER_SCORE = 0;

  constexpr size_t INITIAL_TOWER_RANGE = 3;
  constexpr size_t INITIAL_TOWER_COOLDOWN = 2;

  struct BaseEntityTraits
  {
    size_t health_points = INITIAL_ENTITY_HEALTH_POINTS;
    size_t attack_damage = INITIAL_ENTITY_ATTACK_DAMAGE;
  };

  struct PlayerTraits
  {
    BaseEntityTraits base_traits;
    size_t score = INITIAL_PLAYER_SCORE;
  };

  struct EnemyTraits
  {
    BaseEntityTraits base_traits;
  };

  struct EnemyTowerTraits
  {
    BaseEntityTraits base_traits;
    size_t attack_range = INITIAL_TOWER_RANGE;
    size_t cooldown = INITIAL_TOWER_COOLDOWN;
  };
}

#endif
