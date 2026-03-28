#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstddef>
#include <utility>
#include "entity-traits.hpp"

namespace game
{
  class Field;
  class Entity
  {
  public:
    bool is_alive() const noexcept;

    size_t get_health_points() const noexcept;
    size_t get_attack_damage() const noexcept;
    Position get_position() const noexcept;
    void take_damage(size_t amount) noexcept;

    void set_health_points(size_t amount) noexcept;
    void move_to_pos(Position pos);
    void link_to_field(Field* field_ptr);
  protected:
    size_t health_points_;
    size_t attack_damage_;

    Field* field_ptr_;
    Position pos_;

    Entity(BaseEntityTraits traits, Field* field_ptr, Position pos);
    Entity(const Entity& rhs);
  };
}

#endif
