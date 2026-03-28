#include "enemy.hpp"
#include "base-types.hpp"

game::Enemy::Enemy(EnemyTraits traits, Field* field_ptr, Position pos):
  Entity(traits.base_traits, field_ptr, pos)
{}

game::Enemy::Enemy(const Enemy& rhs):
  Entity(rhs)
{}
