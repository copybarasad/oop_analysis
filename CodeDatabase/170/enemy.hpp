#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"

namespace game
{
  class Enemy: public Entity
  {
  public:
    Enemy(EnemyTraits traits, Field* field_ptr, Position pos);
    Enemy(const Enemy& rhs);
  };
}

#endif
