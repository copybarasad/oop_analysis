#include "entity.hpp"
#include <stdexcept>

game::Entity::Entity(BaseEntityTraits traits, Field* field_ptr, Position pos):
  health_points_(traits.health_points),
  attack_damage_(traits.attack_damage),
  field_ptr_(field_ptr),
  pos_(pos)
{}

game::Entity::Entity(const Entity& rhs):
  health_points_(rhs.health_points_),
  attack_damage_(rhs.attack_damage_),
  field_ptr_(rhs.field_ptr_),
  pos_(rhs.pos_)
{}

bool game::Entity::is_alive() const noexcept
{
  return health_points_ > 0;
}

size_t game::Entity::get_health_points() const noexcept
{
  return health_points_;
}

size_t game::Entity::get_attack_damage() const noexcept
{
  return attack_damage_;
}

game::Position game::Entity::get_position() const noexcept
{
  return pos_;
}

void game::Entity::move_to_pos(Position pos)
{
  pos_ = pos;
}

void game::Entity::link_to_field(Field* field_ptr)
{
  field_ptr_ = field_ptr;
}

void game::Entity::take_damage(size_t amount) noexcept
{
  health_points_ = amount > health_points_ ? 0 : health_points_ - amount;
}

void game::Entity::set_health_points(size_t amount) noexcept
{
  health_points_ = amount;
}
